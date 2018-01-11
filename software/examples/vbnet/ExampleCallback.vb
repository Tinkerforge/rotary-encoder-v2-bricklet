Imports System
Imports Tinkerforge

Module ExampleCallback
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Rotary Encoder Bricklet 2.0

    ' Callback subroutine for count callback
    Sub CountCB(ByVal sender As BrickletRotaryEncoderV2, ByVal count As Integer)
        Console.WriteLine("Count: " + count.ToString())
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim re As New BrickletRotaryEncoderV2(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register count callback to subroutine CountCB
        AddHandler re.CountCallback, AddressOf CountCB

        ' Set period for count callback to 1s (1000ms) without a threshold
        re.SetCountCallbackConfiguration(1000, False, "x"C, 0, 0)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
