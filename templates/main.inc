<html>
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
    <meta name="robots" content="noindex" />
    <style>
      body, html {
        margin:     0;
        padding:    0;
        background: none transparent;
      }

      p { margin: 10px 0; }

      .clear { clear: both; }

      .wrapper {
        width:       300px;
        color:       #555;
        font-family: 'Open Sans', Helvetica, Arial, sans-serif;
        font-size:   12px;
        line-height: 10px;
      }

      .avatar {
        padding:          3px;
        background-color: <?=$profile->color?>;
        border-radius:    3px;
      }

      a {
        color:           <?=$profile->color?>;
        text-decoration: none;
      }
      /* a:hover { text-shadow: 0 0 2px <?=$profile->color?>; } */

      .row {
        width: 100%;
      }

      .col {
        display:        inline-block;
        overflow:       hidden;
        vertical-align: middle;
      }

      .col-1 { width: 16px; }
      .col-2 { width: 32px; }
      .col-3 { width: 48px; }
      .col-4 { width: 64px; }
      .col-5 { width: 80px; }
      .col-6 { width: 96px; }
      .col-7 { width: 112px; }
      .col-8 { width: 128px; }
      .col-9 { width: 144px; }
      .col-10 { width: 160px; }
      .col-11 { width: 176px; }
      .col-12 { width: 192px; }
      .col-13 { width: 208px; }
      .col-14 { width: 224px; }
      .col-15 { width: 240px; }
      .col-16 { width: 256px; }
    </style>
  </head>
  <body>
    <div class="wrapper">
      <div class="row">
        <div class="col col-5">
          <a href="<?=$profile->link?>" target="_blank">
            <img src="<?=$profile->avatar_medium?>" class="avatar" title="<?=$profile->nick?>" />
          </a>
        </div>
        <div class="col col-11">
          <p><a href="<?=$profile->link?>" target="_blank"><?=$profile->nick?></a></p>
          <p>Games owned <?=$profile->game_count?></p>
          <p>Games <em>not</em> played <?=$profile->not_played_count?> (<?=$profile->not_played_perc?>%)</p>
          <p>Since <?=$profile->created?></p>
        </div>
      </div>
      <?php if ($profile->in_game) : ?>
        <p>I'm currently playing</p>
        <a href="<?=$game->link?>" target="_blank" title="<?=$game->name?>">
          <img src="<?=$game->header?>" alt="<?=$game->name?>" width="<?=$width?>" />
        </a>
      <?php else : ?>
        <div class="row">
          <div class="col col-2">&nbsp;</div>
          <div class="col col-14"><p>Recently played game<?=($profile->played_2weeks==1?'':'s')?></p></div>
        </div>
        <?php foreach ($games as $game) : ?>
          <div class="row">
            <a href="<?=$game->link?>" target="_blank">
              <div class="col col-2">
                <img src="<?=$game->image?>" title="<?=$game->name?>" />
              </div>
              <div class="col col-1"><center>&dash;</center></div>
              <div class="col col-13">
                <?=$game->name?>
              </div>
            </a>
          </div>
        <?php endforeach; ?>
      <?php endif; ?>
    </div>
  </body>
</html>
