using Android;
using Android.App;
using Android.Content;
using Android.OS;
using Android.Widget;

namespace application_soignant
{
    [Activity(Label = "SD")]
    public class SD : Activity
    {
        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            SetContentView(Resource.Layout.SD);
            TextView textView = FindViewById<TextView>(Resource.Id.EtatSD);
            Button formater_button = FindViewById<Button>(Resource.Id.Formater_carte);
            Button ejection_button = FindViewById<Button>(Resource.Id.Ejecter_carte);
            ejection_button.Click += delegate
            {
               Intent Alarme_intent = new Intent(this, typeof(Alarme));
                StartActivity(Alarme_intent);
            };

        }
    }
}