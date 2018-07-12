using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ABC.Printer.Decorator
{
    public class FormSecondPageDecorator : BaseFormDataDecorator
    {
        public FormSecondPageDecorator(AbstractFormData abstractFormData)
            :base(abstractFormData)
        {

        }
        public override string FormData(string filename, string str)
        {
            StringBuilder sb = new StringBuilder();
            string a = base.FormData(filename, str);
            sb.Append(a);
            sb.Append("第二联：客户核对\n");
            return sb.ToString();
        }
    }
}
