using System.Threading.Tasks;
using System.Text;
using System.IO;
using System;
using System.Threading;
using application_soignant;

namespace Application_soignant
{
    internal class CCommunication
    {
            Bluetooth bluetooth;
            private Stream inputStream;
            private Stream outputStream;
            private CancellationTokenSource cancellationTokenSource;
            // Événements pour notifier les changements d'état
            public event Action<string> BatteryStatusChanged;
            public event Action<string> SdStatusChanged;
            public event Action<string> IntegerSonorReceived;
            public event Action<string> IntegerAcceReceived;
            public event Action<string> StringValuesReceived;
            public async Task InitializeAsync(string deviceName)
            {
                bluetooth = new Bluetooth();
                inputStream = bluetooth.InputStream;
                outputStream = bluetooth.OutputStream;
                cancellationTokenSource = new CancellationTokenSource();
            await Task.Run(() => Receivedata(cancellationTokenSource.Token));
        }

            public async Task SendData(string data)
            {
                byte[] buffer = Encoding.UTF8.GetBytes(data + "\r\n");
                await outputStream.WriteAsync(buffer, 0, buffer.Length);
            }
            private async void Receivedata(CancellationToken token)
            {
                byte[] buffer = new byte[1024];
                while (!token.IsCancellationRequested)
                {
                    int bytes = await inputStream.ReadAsync(buffer, 0, buffer.Length);
                    if (bytes > 0)
                    {
                        string data = Encoding.UTF8.GetString(buffer, 0, bytes);
                        get_battery(data);
                        get_state_SD(data);
                        receivesonor(data);
                        receiveacce(data);

                    }
                }
            }
         void get_battery(string data)
        {
            if (data.StartsWith("~ b"))
            {
                string batteryStatus = data.Substring(2).Trim();
                BatteryStatusChanged?.Invoke(batteryStatus);
            }
        }
        void get_state_SD(string data)
        {
                if (data.StartsWith("~ s"))
                {
                string sdStatus = data.Substring(2).Trim();
                SdStatusChanged?.Invoke(sdStatus);
             }
        }
        void receivesonor(string data)
        {
            if (data.StartsWith("~ m"))
            {
                string valuesString = data.Substring(2).Trim();
               IntegerSonorReceived?.Invoke(valuesString);
            }
        }
        void receiveacce(string data)
        {
            if (data.StartsWith("~ a"))
            {
                string valuesString = data.Substring(2).Trim();
                IntegerAcceReceived?.Invoke(valuesString);
            }
        }
        void get_name(string data)
        {
            if (data.StartsWith("~ i"))
            {
                string namestring = data.Substring(2).Trim();
                StringValuesReceived?.Invoke(namestring);
            }
        }
            public void Dispose()
            {
                cancellationTokenSource?.Cancel();
            }
        }
    }


