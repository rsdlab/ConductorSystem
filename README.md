名城大学大学院メカトロニクス工学専攻 <br>
コンダクターシステム <br>



各ファイルの説明　<br>

・RTC <br>
　コンダクターシステムで利用するRTコンポーネント群 <br>

    ・Acceleration <br>
 　   LSM6DS33から加速度を取得するコンポーネント <br>

    ・VolumeAdjust <br>
 　  合成加速度によって，音量設定値を出力するコンポーネント<br>

    ・InstrumentSelect <br>
　   iPhoneと通信し，iPhoneアプリの値を出力するコンポーネント <br>

    ・Music_Control <br>
　   ConductorSystem統合コンポーネント <br>

・idl <br>
 Raspberry Pi上でコンダクターシステムを利用するために必要なidl関連ファイル <br>

    ・BasicData <br>
　   VolumeAdjustで利用したコンポーネントのidl関連ファイル <br>

    ・ConductorData <br>
　   InstrumentSelectとMusic_Controlで利用したコンポーネントのidl関連ファイル <br>

    ・MIDIData <br>
　   Music_Controlで利用したコンポーネントのidl関連ファイル <br>

・Instrumetn_Selection <br>
　 iPhoneアプリのプログラム <br>

・Document <br>
　 各種マニュアルが入ったファイル <br>
