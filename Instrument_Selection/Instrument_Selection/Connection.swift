//
//  Connection.swift
//  TCP_IP_Client
//
//  Created by rsdlab on 2017/11/08.
//  Copyright © 2017年 rsdalb. All rights reserved.
//


import Foundation

//各通信処理の挙動の挙動を継承先で定義する
protocol ConnectionDelegate {
    //データの取得が完了した時の処理
    func DidReceivedResponseData(_ response: String)
}

/// ソケット通信クラス
///
class Connection : NSObject, StreamDelegate {
    /// シングルトン
    static let sharedManager = Connection()
    private override init() {
    }
    
    private var serverAddress: CFString! /// 接続先アドレス
    private var serverPort: UInt32!  /// 接続先ポート
    private var inputStream : InputStream! /// 受信用受け口
    private var outputStream : OutputStream! /// 送信用受け口
    var isConnected = false /// 接続状態
    
    private var inputQueue = NSMutableData() /** 受信データのキュー */
    let BUFFER_MAX = 2048 /** 一度に受信するバッファーサイズ */
    
    /** 受信データの委譲先 */
    var delegate: ConnectionDelegate! = nil
    
    /** 汎用エラー */
    static let ERR_MSG:String! = "通信エラー\n再度やり直してください"
    
    /** 接続 */
    func Connect(_ Address: String!, Port: UInt32!) -> Bool{
        //引数の確認
        if( !(Address != nil && Port != nil) ) {
            print("error: serverAddress or serverPort incorrect")
            return false
        }
        
        print("connecting....")
        var readStream : Unmanaged<CFReadStream>?
        var writeStream : Unmanaged<CFWriteStream>?
        
        //serverAddressは16進数表記であるが，同じ文字は入っている．
        serverAddress = Address as CFString
        serverPort = Port
        
        // ソケット作成
        CFStreamCreatePairWithSocketToHost(
            kCFAllocatorDefault,
            serverAddress,
            serverPort,
            &readStream,
            &writeStream)
        
        if( inputStream != nil ) {
            // 接続中の場合は切断
            inputStream.delegate = nil
            inputStream.close()
            inputStream.remove(
                from: RunLoop.current,
                forMode: RunLoopMode.defaultRunLoopMode)
        }
        
        if( outputStream != nil ) {
            // 接続中の場合は切断
            outputStream.delegate = nil
            outputStream.close()
            outputStream.remove(
                from: RunLoop.current,
                forMode: RunLoopMode.defaultRunLoopMode)
        }
        
        inputStream = readStream!.takeRetainedValue() as InputStream
        outputStream = writeStream!.takeRetainedValue() as OutputStream
        
        // ストリームイベントの委譲先
        inputStream.delegate = self
        outputStream.delegate = self
        
        inputStream.schedule(
            in: RunLoop.current,
            forMode: RunLoopMode.defaultRunLoopMode)
        outputStream.schedule(
            in: RunLoop.current,
            forMode: RunLoopMode.defaultRunLoopMode)
        
        inputStream.open()
        outputStream.open()
        print("connect success")
        return true
    }
    
    /** 接続の切断処理を行う */
    func DisConnect() {
        print("ソケット切断")
        inputStream.delegate = nil
        outputStream.delegate = nil
        
        inputStream.close()
        outputStream.close()
        
        inputStream.remove(
            from: RunLoop.current,
            forMode: RunLoopMode.defaultRunLoopMode)
        outputStream.remove(
            from: RunLoop.current,
            forMode: RunLoopMode.defaultRunLoopMode)
        
        isConnected = false
    }
    
    /** ストリームの状態が変化した時に呼ばれる */
    //実行されない　謎
    func StreamChange(_ aStream: Stream, handleEvent eventCode: Stream.Event) {
        if aStream === inputStream {
            // 入力ストリーム
            switch eventCode {
            case Stream.Event.errorOccurred:
                //print("input: ErrorOccurred: \(aStream.streamError?.description)")
                print("1")
            case Stream.Event.openCompleted:
                print("input: OpenCompleted")
            case Stream.Event.hasBytesAvailable:
                print("input: HasBytesAvailable")
                // 入力ストリーム読み込み可能
                GetResponse()
                
            case Stream.Event.endEncountered:
                print("input: EndEncountered")
                // サーバから切断された？
                DisConnect()
            default:
                break
            }
        }
        else if aStream === outputStream {
            // 出力ストリーム
            switch eventCode {
            case Stream.Event.errorOccurred:
                //print("output: ErrorOccurred: \(aStream.streamError?.description)")
                print("1")
            case Stream.Event.openCompleted:
                print("output: OpenCompleted")
            case Stream.Event.hasSpaceAvailable:
                print("output: HasSpaceAvailable")
                print("データ送信可能")
                
                // Here you can write() to `outputStream`
                isConnected = true
                
            case Stream.Event.endEncountered:
                print("output: EndEncountered")
                DisConnect()
                
            default:
                break
            }
        }
    }
    
    // requestを投げる
    func SendRequest(_ msg: String) -> Bool{
        if( serverAddress == nil && serverPort == nil ) {
            print("error: serverAddress or serverPort incorrect")
            return false
        }
        //切断していた場合，再接続
        if !isConnected {
            if(Connect(serverAddress as String, Port: serverPort) == false){
                print("error: can't reconnect")
                return false
            }
        }
        // エンコード
        let request = msg.data(
            using: String.Encoding.shiftJIS,
            allowLossyConversion: false)
        
        ////// メモリの管理
        let charPtr = UnsafeMutablePointer<CUnsignedChar>.allocate(capacity: (request?.count)!)
        charPtr.initialize(from: request!) //初期化
        let dataBytes = UnsafeRawPointer(charPtr) //文字の長さ分メモリを割り振り
        
        //関数を抜ける際に実行
        defer {
            // メモリを解放する
            charPtr.deinitialize(count: (request?.count)!)
            charPtr.deallocate(capacity: (request?.count)!)
        }
        
        let requestBytes: UnsafePointer<UInt8> = dataBytes.bindMemory(to: UInt8.self, capacity: BUFFER_MAX)
        ////////
        
        //送信する長さを取得
        let requestLength = request!.count
        
        var timeout = 5 * 100000 // wait 5 seconds before giving up
        //NSOperationQueue().addOperationWithBlock { [weak self] in
        while !self.outputStream.hasSpaceAvailable {
            usleep(1000) // wait until the socket is ready
            timeout -= 100
            if timeout < 0 {
                print("time out")
                self.delegate.DidReceivedResponseData(Connection.ERR_MSG)
                return false
            } else if self.outputStream.streamError != nil {
                print("disconnect Stream")
                self.delegate.DidReceivedResponseData(Connection.ERR_MSG)
                return false// disconnectStream will be called.
            }
        }
        print("write")
        self.outputStream.write(requestBytes, maxLength: requestLength)
        return true
        //}
    }
    
    /* responseを受け取る */
    private func GetResponse() {
        var buffer = UnsafeMutablePointer<UInt8>.allocate(capacity: BUFFER_MAX)
        var length = self.inputStream!.read(buffer, maxLength: BUFFER_MAX)
        
        if length == -1 {
            print("length:-1")
            return
        }
        // ストリームデータを文字列に変更
        _ = NSString(
            data: NSData(bytes: buffer, length: length) as Data,
            encoding: String.Encoding.shiftJIS.rawValue )
        
        print("length:" + length.description)
        
        // データが断片化する可能性があるのでキューにためておく
        inputQueue.append(NSData(bytes: buffer, length: length) as Data)
        
        let work = inputQueue
        
        buffer = UnsafeMutablePointer<UInt8>.allocate(capacity: BUFFER_MAX)
        length = work.length
        
        //バイナリを文字列型に変換
        let allStream = NSString(
            data: NSData(bytes: buffer, length: length) as Data,
            encoding: String.Encoding.shiftJIS.rawValue )
        
        if (allStream != nil) {
            if( allStream?.length == 0 ) {
                print("切断された？")
            }
            
            print("データ受信完了")
            let data: String = allStream! as String
            
            // データ受信完了後に委譲先に処理を依頼
            if( delegate == nil ) {
                print("委譲先を設定してください")
            }
            
            delegate.DidReceivedResponseData(data)
            inputQueue = NSMutableData()
        }
    }
}

