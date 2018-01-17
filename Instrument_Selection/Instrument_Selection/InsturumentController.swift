//
//  InsturumentController.swift
//  Instrument_Selection
//
//  Created by rsdlab on 2017/11/17.
//  Copyright © 2017年 rsdalb. All rights reserved.
//

import UIKit

class InsturumentController: UIViewController, ConnectionDelegate {
    func DidReceivedResponseData(_ response: String) {
        
    }
    
    var instswitch = ["0", "0", "0", "0"]
    let connection = Connection.sharedManager
    
    @IBOutlet weak var instrument1: UISwitch!
    @IBOutlet weak var instrument2: UISwitch!
    @IBOutlet weak var instrument3: UISwitch!
    @IBOutlet weak var instrument4: UISwitch!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        connection.delegate = self
        
        instrument1.isOn = false
        instrument2.isOn = false
        instrument3.isOn = false
        instrument4.isOn = false
    }

    /*
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
*/

    @IBAction func InstrumentBtn1(_ sender: UISwitch) {
        if sender.isOn{
            instswitch[0] = "1"
            print("switch1::on")
        }else{
            instswitch[0] = "0"
            print("switch1::off")
        }
        let sendmes = instswitch[0] + instswitch[1] + instswitch[2] + instswitch[3]
        if(connection.SendRequest(sendmes) == false){
            print("not send")
        }
    }
    
    @IBAction func InstrumentBtn2(_ sender: UISwitch) {
        if sender.isOn{
            instswitch[1] = "1"
            print("switch2::on")
        }else{
            instswitch[1] = "0"
            print("switch2::off")
        }
        let sendmes = instswitch[0] + instswitch[1] + instswitch[2] + instswitch[3]
        if(connection.SendRequest(sendmes) == false){
            print("not send")
        }
    }
    
    @IBAction func InstrumentBtn3(_ sender: UISwitch) {
        if sender.isOn{
            instswitch[2] = "1"
            print("switch3::on")
        }else{
            instswitch[2] = "0"
            print("switch3::off")
        }
        let sendmes = instswitch[0] + instswitch[1] + instswitch[2] + instswitch[3]
        if(connection.SendRequest(sendmes) == false){
            print("not send")
        }
    }
    
    @IBAction func InstrumentBtn4(_ sender: UISwitch) {
        if sender.isOn{
            instswitch[3] = "1"
            print("switch4::on")
        }else{
            instswitch[3] = "0"
            print("switch4::off")
        }
        let sendmes = instswitch[0] + instswitch[1] + instswitch[2] + instswitch[3]
        if(connection.SendRequest(sendmes) == false){
            print("not send")
        }
    }
    
    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
    }
    */

}
