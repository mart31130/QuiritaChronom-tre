using Android.Bluetooth;
using Application_soignant;
namespace application_soignant
{
    internal class CQuirita
    {
        BluetoothAdapter bluetoothAdapter;
        CCommunication CComm = new CCommunication();
        // recupére le niveau de batterie du dispositif connecté en bluetooth en temps réel
       // public int get_battery()
           // {
           //     CComm.get_battery();
           // }
            public string get_name()
            {
                return bluetoothAdapter?.Name;
            }
            public void Change_name(string name)
            {
                if (bluetoothAdapter != null && bluetoothAdapter.IsEnabled)
                {
                    bluetoothAdapter.SetName(name);
                }
            }
        }
    }