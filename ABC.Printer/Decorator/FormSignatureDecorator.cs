using ABC.HelperClass;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ABC.Printer.Decorator
{
    /// <summary>
    /// 标题装饰器
    /// </summary>
    public class FormSignatureDecorator : BaseFormDataDecorator
    {
        public FormSignatureDecorator(AbstractFormData formData)
            : base(formData)
        {
        }
        public override string FormData(string filename, string str)
        {
            StringBuilder strb = new StringBuilder();
            string a = base.FormData(filename, str);
            strb.Append(a);
            strb.Append("\n");
            strb.Append("\n");
            strb.Append("\n");
            strb.Append("客户签名：\n");
            return strb.ToString();
        }
    }
}
