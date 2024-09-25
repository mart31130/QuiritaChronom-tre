using Android;
using Android.App;
using Android.Bluetooth;
using Android.Content;
using Android.OS;
using Android.Widget;
using Application_soignant;
using Java.Util;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;

namespace application_soignant
{
    [Activity(Label = "@string/app_name", Theme = "@style/AppTheme", MainLauncher = true)]
    public class Bluetooth : Activity
    {
        // Instanciation des variables
        BluetoothAdapter bluetoothAdapter;
        BluetoothDevice device;
        BluetoothSocket socket;
        public Stream InputStream { get; private set; }
        public Stream OutputStream { get; private set; }
        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            SetContentView(Application_soignant.Resource.Layout.Bluetooth_activity);
            Button button = FindViewById<Button>(Application_soignant.Resource.Id.Bluetooth_list);
            button.Click += RechercheBluetooth_Click;

            // Configuration du Bluetooth
            var bluetoothManager = (BluetoothManager)GetSystemService(Context.BluetoothService);
            bluetoothAdapter = bluetoothManager?.Adapter;
            if (bluetoothAdapter == null)
            {
                Toast.MakeText(this, "Le Bluetooth n'est pas supporté sur cet appareil", ToastLength.Long).Show();
            }
            else if (!bluetoothAdapter.IsEnabled)
            {
                Intent btintent = new Intent(BluetoothAdapter.ActionRequestEnable);
                StartActivityForResult(btintent, 1);
            }
            else
            {
                Task.Run(BluetoothConnexion);
            }
        }

        private void RechercheBluetooth_Click(object sender, EventArgs e)
        {
            BluetoothConnexion();
        }

        private async void BluetoothConnexion()
        {
            await Task.Run(() =>
            {
                try
                {
                    // Recherche des appareils Bluetooth disponibles
                    var allDevices = bluetoothAdapter.BondedDevices;
                    if (allDevices.Count > 0)
                    {
                        // Convertir la liste d'appareils en une liste de noms pour l'affichage
                        List<string> deviceNames = new List<string>();
                        foreach (var device in allDevices)
                        {
                            deviceNames.Add(device.Name);
                        }

                        // Afficher les appareils dans une liste déroulante (Spinner)
                        RunOnUiThread(() =>
                        {
                            AlertDialog.Builder builder = new AlertDialog.Builder(this);
                            builder.SetTitle("Choisir un appareil Bluetooth");
                            builder.SetItems(deviceNames.ToArray(), (sender, args) =>
                            {
                                int selectedDeviceIndex = args.Which;
                                BluetoothDevice selectedDevice = allDevices.ElementAt(selectedDeviceIndex);

                                ConnectToDevice(selectedDevice);
                            });
                            builder.Show();
                        });
                    }
                    else
                    {
                        RunOnUiThread(() =>
                        {
                            Toast.MakeText(this, "Aucun appareil Bluetooth trouvé.", ToastLength.Long).Show();
                        });
                    }
                }
                catch (IOException ex)
                {
                    RunOnUiThread(() =>
                    {
                        Toast.MakeText(this, "Erreur lors de la connexion Bluetooth: " + ex.Message, ToastLength.Long).Show();
                    });
                }
            });
        }

        private void ConnectToDevice(BluetoothDevice device)
        {
            try
            {
                // Création d'un socket RFCOMM avec le périphérique Bluetooth sélectionné
                socket = device.CreateRfcommSocketToServiceRecord(UUID.FromString("00001101-0000-1000-8000-00805F9B34FB"));

                // Connecter le socket de manière asynchrone
                socket.ConnectAsync().ContinueWith(task =>
                {
                    if (task.IsCompleted && !task.IsFaulted)
                    {
                        RunOnUiThread(() =>
                        {
                            Toast.MakeText(this, "Connexion Bluetooth établie avec succès!", ToastLength.Long).Show();
                        });
                        Intent intent = new Intent(this, typeof(MainActivity));
                        StartActivity(intent);
                    }
                    else
                    {
                        RunOnUiThread(() =>
                        {
                            Toast.MakeText(this, "Erreur lors de la connexion Bluetooth.", ToastLength.Long).Show();
                        });
                    }
                });
            }
            catch (IOException ex)
            {
                RunOnUiThread(() =>
                {
                    Toast.MakeText(this, "Erreur lors de la connexion Bluetooth: " + ex.Message, ToastLength.Long).Show();
                });
            }
        }
    }
}

