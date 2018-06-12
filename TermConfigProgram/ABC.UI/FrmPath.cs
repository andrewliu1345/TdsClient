using ABC.Config;
using System.Windows;
using System.Windows.Forms;

namespace ABC.UI
{
    /// <summary>
    /// FrmPrinter.xaml 的交互逻辑
    /// </summary>
    public partial class FrmPath : Window
    {
        PathClass path;
        public FrmPath()
        {
            InitializeComponent();
            path = AppConfig.Instance.Path;
            txtPath.Text = path.FormPath;
            txtPath2.Text = path.PhotoPath;
        }

        private void btnBrowse_Click(object sender, RoutedEventArgs e)
        {
            txtPath.Text = FolderBrowser(txtPath.Text.Trim());

        }

        private void btnBrowse2_Click(object sender, RoutedEventArgs e)
        {
            txtPath2.Text = FolderBrowser(txtPath2.Text.Trim());
        }
        private string FolderBrowser(string Inpath)
        {
            var oupath = string.Empty;
            FolderBrowserDialog dlg = new FolderBrowserDialog();
            dlg.SelectedPath = Inpath;
            if (dlg.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                oupath = dlg.SelectedPath;
            }
            return oupath;
        }

        private void btnSave_Click(object sender, RoutedEventArgs e)
        {
            path.FormPath = txtPath.Text;
            path.PhotoPath = txtPath2.Text;
            path.Save();
            System.Windows.Forms.MessageBox.Show("保存成功");
        }
    }
}
