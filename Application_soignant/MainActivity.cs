using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views.Animations;
using Android.Widget;
using application_soignant;
using Xamarin.Essentials;
using System.Threading.Tasks;
namespace Application_soignant
{
    [Activity(Label = "@string/app_name", Theme = "@style/AppTheme")]
    public class MainActivity : Activity
    {
        CCommunication Communication;
        private ProgressBar Batterie_progressBar;
        private TextView Etat_batterie;
        private ProgressBar SD_progressBar;
        private TextView Etat_SD;
        private TextView ID;
        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            SetContentView(Resource.Layout.activity_main);

            // clique sur le bouton alarme
            Button button = FindViewById<Button>(Resource.Id.Alarme);
            button.Click += delegate
            {
                var animation = new ScaleAnimation(
                    1.0f, 1.2f,
                    1.0f, 1.2f,
                    Dimension.RelativeToSelf, 0.5f,
                    Dimension.RelativeToSelf, 0.5f);
                animation.Duration = 300;
                animation.RepeatCount = 1;
                animation.RepeatMode = RepeatMode.Reverse;
                button.StartAnimation(animation);
                Intent Alarme_intent = new Intent(this, typeof(Alarme));
                StartActivity(Alarme_intent);
            };

            //affichage de l'état de la batterie
            Batterie_progressBar = FindViewById<ProgressBar>(Resource.Id.battery);
            Etat_batterie = FindViewById<TextView>(Resource.Id.Etatbatterie);
            Communication = new CCommunication();
            Communication.BatteryStatusChanged += UpdateBatteryStatus;


            //clique sur le bouton SD
            SD_progressBar = FindViewById<ProgressBar>(Resource.Id.sd);
            Etat_SD = FindViewById<TextView>(Resource.Id.etatsd);
            Communication.SdStatusChanged += UpdateSDStatus;

            //clique sur le textview ID
            ID = FindViewById<TextView>(Resource.Id.ChangeID);
            Communication.StringValuesReceived += UpdateID;
            ID.Click += delegate
            {
                ShowIDDialog();
            };
        }
        private void UpdateBatteryStatus(string status)
        {
            // Convertir le niveau de batterie reçu en entier
            if (int.TryParse(status, out int batteryLevel))
            {
                RunOnUiThread(() =>
                {
                    Batterie_progressBar.Progress = batteryLevel;
                    Etat_batterie.Text = $"Etat batterie : {batteryLevel}%";
                });
            }
        }
        private void UpdateSDStatus(string status)
        {
            // Convertir le niveau de batterie reçu en entier
            if (int.TryParse(status, out int SDLevel))
            {
                RunOnUiThread(() =>
                {
                    SD_progressBar.Progress = SDLevel;
                    Etat_SD.Text = $"Etat carte SD : {SDLevel}%";
                });
            }
        }
        private void UpdateID(string id)
        {
            RunOnUiThread(() =>
            {
                ID.Text = id;
            });
        }
        private void ShowIDDialog()
        {
            var dialog = new AlertDialog.Builder(this);
            dialog.SetTitle("Entrer le nom de la chambre");
            dialog.SetCancelable(false); // permet à la boite de dialogue de rester affichée, si l'utilisateur clique à côté

            EditText ID_input = new EditText(this)
            {
            };
            dialog.SetView(ID_input);
            dialog.SetPositiveButton("Ok", (sender, args) =>
            {
                string newvalue = ID_input.Text;
                RunOnUiThread(() =>
                {
                    ID.Text = newvalue;
                });
                Task task = Communication.SendData($"db set QuiritaIdentifiant {newvalue}");
            });
            dialog.SetNegativeButton("Annuler", (sender, args) => { });
            dialog.Show();
        }
        public override void OnRequestPermissionsResult(int requestCode, string[] permissions, [GeneratedEnum] Android.Content.PM.Permission[] grantResults)
        {
            Xamarin.Essentials.Platform.OnRequestPermissionsResult(requestCode, permissions, grantResults);
            base.OnRequestPermissionsResult(requestCode, permissions, grantResults);
        }
    }
}

