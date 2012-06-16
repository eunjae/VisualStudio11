using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;

namespace SL5MediaPlayer_1
{
    public partial class MainPage : UserControl
    {
        public MainPage()
        {
            InitializeComponent();
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            MediaElement1.Play();
            Label1.Content = "Playing.....";
        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            MediaElement1.Pause();
            Label1.Content = "Paused.....";
        }

        private void Button_Click_3(object sender, RoutedEventArgs e)
        {
            MediaElement1.Stop();
            Label1.Content = "Stopped.....";
        }
    }
}
