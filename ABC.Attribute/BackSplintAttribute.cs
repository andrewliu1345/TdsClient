using System;

namespace ABC.Attribute
{
    [AttributeUsage(AttributeTargets.Class)]
    public class BackSplintAttribute : System.Attribute
    {
        public bool isBackSplint
        {
            get; set;
        }
    }
}
