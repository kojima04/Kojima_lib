# Kojima_lib
VC++のソリューション一式です。  
DirectXのライブラリを使用しているのでWindows以外で動作させたい場合はWindowsの仮想環境等を利用してください。  
また、ビルドをするためには以下手順が必要です。

1.「.NetFrameWork 」および「DirectX SDK」のインストール  
2.プロジェクトのプロパティ修正
・「VC++ディレクトリ」の「インクルードディレクトリ」「ライブラリディレクトリ」にDirectX SDKの「Include」「Lib」のパス追加

また、動作確認バージョンは以下の通りです。  
・Windows 7 Professional  
・Visual Studio 2010 Professional  
・.NetFramework4.0  
・Microsoft DirectX SDK(August 2009)  

■ライブラリの概要
DirectXに必要な処理と汎用的な処理をまとめました。

■説明

DirectX:  
DirectXを利用した以下機能がまとめてあります。
・2D/3D描画
・Wavファイルの再生  
・キーボード/ジョイパッド入力の取得  

General：  
汎用的な処理がまとめてあります。
