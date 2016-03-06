<?php
/* Steam Web API sandbox
* @author  Johan / Asbra <johan@asbra.net>
* @date    2013-05-24
*/
$type = isset($_GET['t']) ? intval($_GET['t']) : 1; // 1 = HTML, 2 = Image
$format = isset($_GET['f']) ? strtolower($_GET['f']) : 'png';

$width = isset($_GET['w']) ? intval($_GET['w']) : ($type == 2 ? 640 : 248);
$height = isset($_GET['h']) ? intval($_GET['h']) : ($type == 2 ? 96 : 280);
$count = isset($_GET['c']) ? intval($_GET['c']) : 5;

$profile = new StdClass();
$games = array();

if ($type != 2) {
    // HTML
    header('Content-Type: text/html; charset=UTF-8');

    ob_start();
} else {
    $count = 30;
    header("Content-Type: image/{$format}");
}

require 'steam-header.inc';

if (!isset($_GET['debug']) && $type == 2 && file_exists("cache/{$steam_id}-{$width}x{$height}.{$format}")) {
    // Image
    if ((time() - filemtime("cache/{$steam_id}-{$width}x{$height}.{$format}")) <= (1 * 60) * 60) {
        // if data is more than 1 hour old

        $image = imagecreatefrompng("cache/{$steam_id}-{$width}x{$height}.{$format}");
        imagealphablending($image, false);
        imagesavealpha($image, true);
        imagepng($image);
        imagedestroy($image);
        exit;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Get data

$games_num = $owned->response->game_count;

$player = $summary->response->players[0];

$profile->nick          = $player->personaname;
$profile->link          = $player->profileurl;
$profile->avatar        = $player->avatar;
$profile->avatar_medium = $player->avatarmedium;
$profile->country       = isset($player->loccountrycode) ? $player->loccountrycode : '';
$profile->clan          = isset($player->primaryclanid) ? $player->primaryclanid : 0;
$profile->created       = isset($player->timecreated) ? date('Y-m-d', $player->timecreated) : 0;
$profile->seen          = isset($player->lastlogoff) ? $player->lastlogoff : null;
$profile->state         = $player->personastate;
$profile->in_game       = isset($player->gameid) ? true : false;
$profile->game_count    = $owned->response->game_count;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Set text color

if ($profile->in_game) {
    $profile->color = 'rgb(131,179,89)';
} else {
    switch ($profile->state) {
        case PersonaState::Online:
        case PersonaState::Busy:
        case PersonaState::Away:
        case PersonaState::Snooze:
        case PersonaState::LookingToTrade:
        case PersonaState::LookingToPlay:
            $profile->color = '#4787A0';
            break;
        case PersonaState::Offline:
        default:
            $profile->color = 'grey';
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Get not played games count

$profile->not_played_count = 0;

for ($i = 0; $i < $profile->game_count; ++$i) {
    $game = $owned->response->games[$i];

    if ($game->playtime_forever <= 0) {
        ++$profile->not_played_count;
    }
}

$profile->not_played_perc = round(($profile->not_played_count / $profile->game_count) * 100);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Get played recently (2 weeks)

$profile->played_2weeks = $played->response->total_count;

$max = ($played->response->total_count < $count ? $played->response->total_count : $count);

for ($i = 0; $i < $max; ++$i) {
    $game = $played->response->games[$i];

    if (strlen($game->name) > 32) {
        $game->name = substr($game->name, 0, 30).'..';
    }

    $game->link = "http://steamcommunity.com/app/{$game->appid}";
    $game->image = "http://media.steampowered.com/steamcommunity/public/images/apps/{$game->appid}/{$game->img_icon_url}.jpg";

    $games[] = $game;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// If we're in a game, get info about it

if ($profile->in_game) {
    $game->appid = $player->gameid;
    $game->link = "http://steamcommunity.com/app/{$game->appid}";
    $game->image = "http://media.steampowered.com/steamcommunity/public/images/apps/{$game->appid}/{$game->img_icon_url}.jpg";
    $game->logo = "http://media.steampowered.com/steamcommunity/public/images/apps/{$game->appid}/{$game->img_logo_url}.jpg";
    $game->header = "http://cdn.akamai.steamstatic.com/steam/apps/{$game->appid}/header.jpg";
    $game->name = $player->gameextrainfo;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Load the template

if ($type == 2) {
    require 'templates/image.inc';
} else {
    if (file_exists('templates/main.min.inc')) {
        require 'templates/main.min.inc';
    } else {
        require 'templates/main.inc';
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Save output and display it

    $output = ob_get_clean();
    // ob_flush();
    echo $output;

    $output = time()."\r\n".$output;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Save to cache

    file_put_contents("cache/{$steam_id}.html", $output);
}
