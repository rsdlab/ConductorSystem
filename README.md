名城大学大学院メカトロニクス工学専攻
コンダクターシステム



各ファイルの説明

・RTC
　コンダクターシステムで利用するRTコンポーネント群

    ・Acceleration
 　   LSM6DS33から加速度を取得するコンポーネント

    ・VolumeAdjust
 　  合成加速度によって，音量設定値を出力するコンポーネント

    ・InstrumentSelect
　   iPhoneと通信し，iPhoneアプリの値を出力するコンポーネント

    ・Music_Control
　   ConductorSystem統合コンポーネント

・idl
 Raspberry Pi上でコンダクターシステムを利用するために必要なidl関連ファイル

    ・BasicData
　   VolumeAdjustで利用したコンポーネントのidl関連ファイル

    ・ConductorData
　   InstrumentSelectとMusic_Controlで利用したコンポーネントのidl関連ファイル

    ・MIDIData
　   Music_Controlで利用したコンポーネントのidl関連ファイル

・Instrumetn_Selection
　 iPhoneアプリのプログラム

・Document
　 各種マニュアルが入ったファイル
