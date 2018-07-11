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
    public class FormHeaderDecorator : BaseFormDataDecorator
    {
        public FormHeaderDecorator(AbstractFormData formData)
            : base(formData)
        {
        }
        public override string FormData(string filename, string str)
        {
            StringBuilder strb = new StringBuilder();
            strb.Append("         中国农业银行\n");
            string a = base.FormData(filename, str);
            strb.Append(a);
            return strb.ToString() ;
        }
    }
}
