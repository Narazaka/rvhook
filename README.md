rvhook
======
an application which hooks starting the game of rpgvxace up.
rvhook: rpgvxaceがGame.exeを起動する際に別の処理をフックするためのツールです。

## 使い方
rpgvxaceのプロジェクトを作成後、次の作業を行ってください:
* Game.exeを_Game.exeにリネームする
* Game.iniを_Game.iniにリネームする
* rvhookのGame.exeを、作成したプロジェクトのフォルダにコピーする
* hook_preboot.bat及びhook_postboot.batというファイルを、作成したプロジェクトのフォルダに作成する

## 挙動
1. rpgvxaceのエディタからゲームを実行しようとするとrvhookのGame.exeが起動します
2. rvhookのGame.exeはhook.batを実行します。  
ゲーム起動時に実行したい処理がある場合は、このバッチファイルに記述してください。
3. rvhookのGame.exeは、_Game.exe(本来のGame.exe)を実行します

# 更新履歴
* 2.0.0	修正
* 1.0.0	最初のバージョン

