using System.Threading.Tasks;
using Android.App;
using Android.OS;
using Android.Widget;
using Application_soignant;

namespace application_soignant
{
    [Activity(Label = "Alarme")]
    public class Alarme : Activity
    {
        CCommunication CCommunication;
        private TextView agitationTextView;
        private TextView bruitTextView;
        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            SetContentView(Application_soignant.Resource.Layout.Alarme_activity);
            CCommunication = new CCommunication();
            agitationTextView = FindViewById<TextView>(Application_soignant.Resource.Id.Agitation);
            CCommunication.IntegerAcceReceived += ReceiveAgitationValue;
            agitationTextView.Click += delegate
            {
                ShowAgitationDialog();
            };

            bruitTextView = FindViewById<TextView>(Application_soignant.Resource.Id.Bruit);
             CCommunication.IntegerSonorReceived += ReceiveBruitValue;

              bruitTextView.Click += delegate
                {
                    ShowBruitDialog();
                };
        }
        private void ReceiveAgitationValue(string status)
        {
            // Convertir le niveau d'agitation reçu en entier
            if (int.TryParse(status, out int agitationValue))
            {
                RunOnUiThread(() =>
                {
                    agitationTextView.Text = $"Valeur d'agitation : {agitationValue}%";
                });
            }
        }
        private void ReceiveBruitValue(string status)
        {
            // Convertir le niveau de bruit reçu en entier
            if (int.TryParse(status, out int bruitValue))
            {
                RunOnUiThread(() =>
                {
                    bruitTextView.Text = $"Valeur de bruit : {bruitValue}%";
                });
            }
        }
        private void ShowAgitationDialog()
        {
            var dialog = new AlertDialog.Builder(this);
            dialog.SetTitle("Entrer la valeur de l'agitation");
            dialog.SetCancelable(false); // permet à la boite de dialogue de rester affichée, si l'utilisateur clique à côté

            EditText agitation_input = new EditText(this)
            {
                InputType = Android.Text.InputTypes.ClassNumber // l'utilisateur ne peut rentrer que des entiers
             };
            dialog.SetView(agitation_input);
            dialog.SetPositiveButton("Ok", (sender, args) =>
            {
                string newvalue = agitation_input.Text;
               RunOnUiThread(() =>
             {
                 agitationTextView.Text = newvalue;
             });
             Task task = CCommunication.SendData($" db set seuilAcce{newvalue}");
            });
            dialog.SetNegativeButton("Annuler", (sender, args) => {});
            dialog.Show();
        }
        private void ShowBruitDialog()
        {
            var dialog = new AlertDialog.Builder(this);
            dialog.SetTitle("Entrer la valeur du bruit");
            dialog.SetCancelable(false); // permet à la boite de dialogue de rester affichée, si l'utilisateur clique à côté

            EditText bruit_input = new EditText(this)
            {
                InputType = Android.Text.InputTypes.ClassNumber 
            };
            dialog.SetView(bruit_input);
            dialog.SetPositiveButton("Ok", (sender, args) =>
            {
                string newvalue = bruit_input.Text;
                RunOnUiThread(() =>
                {
                    bruitTextView.Text = newvalue;
                });
                Task task = CCommunication.SendData($" db set sonorSeuil{newvalue}");
            });
            dialog.SetNegativeButton("Annuler", (sender, args) => { });
            dialog.Show();
        }
    }
}
