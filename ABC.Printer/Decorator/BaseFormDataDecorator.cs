using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ABC.Printer.Decorator
{
    public class BaseFormDataDecorator : AbstractFormData
    {
        private AbstractFormData _formData = null;
        public BaseFormDataDecorator(AbstractFormData formData)
        {
            this._formData = formData;
        }

        public override string FormData(string filename, string str)
        {
            return _formData.FormData(filename, str);
        }
    }
}
