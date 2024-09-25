using Android;
using Android.App;
using Android.OS;
using Android.Widget;
using System;
namespace application_soignant
{
    [Activity(Label = "ID")]
    public class ID : Activity
    {
        EditText editText;
        Button Validate;
        CQuirita quirita ;
        TextView device_name;
        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);

            SetContentView(Resource.Layout.ID);
            // interraction avec l'identifiant
            editText = FindViewById<EditText>(Resource.Id.Id_quiri);
            Validate = FindViewById<Button>(Resource.Id.Valider);
            Validate.Click += Change_name;
                get_name();
            Button backButton = FindViewById<Button>(Resource.Id.retour);
            backButton.Click += delegate
            {
                Finish();
            };
            quirita = new CQuirita();
        }
        void get_name()
        {
            device_name.Text = quirita.get_name();
        }
        private void Change_name(object sender, EventArgs e)
        {
            string enteredId = editText.Text;

            if (string.IsNullOrEmpty(enteredId))
            {
                Toast.MakeText(this, "Veuillez entrer un texte", ToastLength.Short).Show();
                return;
            }
            quirita.Change_name(enteredId);
            if (quirita.get_name() == enteredId)
            {
                Toast.MakeText(this, "Nom Bluetooth modifié", ToastLength.Short).Show();
                device_name.Text = "Current Bluetooth Name: " + enteredId;
            }
        }
    }
}
