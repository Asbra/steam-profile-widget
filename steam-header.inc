<?php
/*
 * @author  Johan / Asbra <johan@asbra.net>
 * @date    2014-02-04
 */
date_default_timezone_set('UTC');

$api_key = '';
$steam_id = '';

// Check if profile ID is given
if (!isset($_GET['p'])) {
    echo <<<END
Enter your Steam profile ID, nick or URL (eg. <a href="https://steamcommunity.com/id/novoc/">novoc</a>)
<br />
<form method="GET">
  <input type="text" name="p" />
  <br />
  <input type="submit" value="Ok" />
</form>
END;
    exit;
}

function SteamApiCall($interface, $method, $version, $api_key, $variables) {
    try {
        $vars = http_build_query($variables);
        $url = "https://api.steampowered.com/{$interface}/{$method}/{$version}/?key={$api_key}&{$vars}";
        return json_decode(file_get_contents($url));
    } catch (Exception $e) {}
    return null;
}

class PersonaState
{
    const Offline        = 0;
    const Online         = 1;
    const Busy           = 2;
    const Away           = 3;
    const Snooze         = 4;
    const LookingToTrade = 5;
    const LookingToPlay  = 6;
}

$profile_url = $_GET['p'];

// If input is already numeric, assume it's a valid Steam ID
if (is_numeric($profile_url)) {
    $steam_id = $profile_url;
}
// If the input is a profile URL, we extract the vanity ID
elseif (strstr($profile_url, 'steamcommunity.com/id/') !== false) {
    $profile_url = rtrim($profile_url, '/');
    $profile_url = substr($profile_url, strrpos($profile_url, '/') + 1);

    if (is_numeric($profile_url)) {
        $steam_id = $profile_url;
    } else {
        $json = SteamApiCall('ISteamUser', 'ResolveVanityURL', 'v0001', $api_key, ['vanityurl' => $profile_url]);

        if ($json->response->success != 1) {
            die('Failed to resolve nickname.');
        }

        $steam_id = $json->response->steamid;
    }
}
// Otherwise assume it's just the vanity ID, so resolve it
else {
    if (strpos($profile_url, '://') === false) {
        $json = SteamApiCall('ISteamUser', 'ResolveVanityURL', 'v0001', $api_key, ['vanityurl' => $profile_url]);

        if ($json->response->success != 1) {
            die('Failed to resolve nickname.');
        }

        $steam_id = $json->response->steamid;
    }
}

if (!is_numeric($steam_id)) {
    die('Steam ID error');
}

// If we're in debug mode, skip cache
if (!isset($_GET['debug']) && file_exists("cache/{$steam_id}.html")) {
    if ($type != 2) {
        $data = file_get_contents("cache/{$steam_id}.html");

        if (isset($data) && !empty($data) && $data != 'null' && strpos($data, "\r\n") !== false) {
            $parts = explode("\r\n", $data);

            $time = intval($parts[0]);
            unset($parts[0]);

            $output = implode("\r\n", $parts);

            if ((time() - $time) > 60 * 10) {
                // if data is more than 10 minutes old

        $summary = array();
                $played = array();
                unset($summary);
                unset($played);
            } else {
                die($output);
            }
        }
    }
}

$played = SteamApiCall('IPlayerService', 'GetRecentlyPlayedGames', 'v0001', $api_key, ['steamid' => $steam_id, 'format' => 'json', 'count' => $count]);
$summary = SteamApiCall('ISteamUser', 'GetPlayerSummaries', 'v0002', $api_key, ['steamids' => $steam_id]);
$owned = SteamApiCall('IPlayerService', 'GetOwnedGames', 'v0001', $api_key, ['steamid' => $steam_id, 'include_played_free_games' => 0, 'include_appinfo' => 1]);

if (!$summary || !$summary->response || !$played || !$played->response) {
    die('Failed to get profile data!');
}
