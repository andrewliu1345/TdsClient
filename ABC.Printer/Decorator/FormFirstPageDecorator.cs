using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ABC.Printer.Decorator
{
    public class FormFirstPageDecorator : BaseFormDataDecorator
    {
        public FormFirstPageDecorator(AbstractFormData abstractFormData)
            :base(abstractFormData)
        {

        }
        public override string FormData(string filename, string str)
        {
            StringBuilder sb = new StringBuilder();
            string a = base.FormData(filename, str);
            sb.Append(a);
            sb.Append("第一联：银行留存\n");
            return sb.ToString();
        }
    }
}
