//
//  ViewController.swift
//  TCP_IP_Client
//
//  Created by rsdlab on 2017/11/08.
//  Copyright © 2017年 rsdalb. All rights reserved.
//

import UIKit

class ViewController: UIViewController, ConnectionDelegate {
    //データの取得が完了した時の処理 できてない
    //eroor メッセージ
    func DidReceivedResponseData(_ response: String) {
        print("受信データ確認：" + response)
        execution.text! = execution.text + response
    }
    
    //TCP_IPのクラス適用
    let tcp_client = Connection.sharedManager
    
    //ボタンの宣言
    @IBOutlet weak var ipText: UITextField!
    
    @IBOutlet weak var portText: UITextField!
    @IBOutlet weak var execution: UITextView!
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        tcp_client.delegate = self
        ipText.borderStyle = .roundedRect
        ipText.keyboardType = UIKeyboardType.decimalPad
        
        portText.borderStyle = .roundedRect
        portText.keyboardType = UIKeyboardType.numberPad
        
        execution.isEditable = false;
        execution.text = nil
        execution.layer.borderColor = UIColor.black.cgColor
        execution.layer.borderWidth = 1.0
    }
    
    /*
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
 */
    
    @IBAction func connectBtn1(_ sender: UIButton) {
        let serverAddress = ipText.text! as String
        let serverPort = UInt32(portText.text!)
        
        if(tcp_client.Connect(serverAddress, Port: serverPort) == false){
            execution.text = execution.text + "Connect Failed \n"
            print("not connect")
        }

    }
    /*
    @IBAction func sendBtn(_ sender: UIButton) {
        //acceptText.text = nil
        if(tcp_client.SendRequest(sendTextField.text!) == false){
            print("not send")
        }else{
            executionText.text! = "send success :: " + sendTextField.text!
        }
    }
    */
}
