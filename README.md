# Steam profile widget
Use Steam Web API to generate a small widget with account statistics (with caching)

Project contains 2 templates, one HTML and one image; [HTML widget](https://dev.asbra.net/steam/?p=76561198002358405) [Image widget](https://dev.asbra.net/steam/?p=76561198002358405&t=2)

*Warning* code is somewhat sloppy, partially commented, and there is little-to-no error handling.

[Article/Tutorial for this project](https://asbra.net/php-steam-profile-widget/) (link to my blog)

[h3]Installation[/h3]
* Edit [steam-header.inc](steam-header.inc) set `$api_key` to your [Steam Web API key](https://steamcommunity.com/dev/apikey)
* Upload to server
* Give write permissions on `cache/` folder, .html cache will be saved there
* Give write permissions on `icons/` folder, game icons/images will be saved there

[h4]GET Variables[/h4]
`debug` if set, skips cache
`t` type of widget/output (1 = HTML, 2 = PNG image)
`w` width (default 248px for HTML, 640px for image)
`h` height (default 280px for HTML, 96px for image)
`f` image file format (only PNG for now)
`c` count of games to show (in HTML output)
`p` Steam profile URL, ID or nick

[h4]Information[/h4]
Set GET variable `debug` if you want to skip cache.

Cache is kept for 1 hour

[steam-header.inc](steam-header.inc) contains the Steam Web API calls
[index.php](index.php) entry point, calls steam-header.inc to get data, handles templating

[h4]Templates[/h5]
[templates](templates/) folder contains templates, here you change the appearance of the widget
Edit `main.inc`, minify it to `main.min.inc` and it will be used. If `main.min.inc` doesn't exist, `main.inc` is used as template.
