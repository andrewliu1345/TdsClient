﻿using ABC.Config;
using ABC.HelperClass;
using ABC.Logs;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Xml.Linq;

namespace ABC.Printer
{
    public class LoadFormData:AbstractFormData
    {
        private string path = AppConfig.Instance.Path.FormPath;
        public LoadFormData()
        {

        }
        public override string FormData(string filename, string str)
        {
           
            string file = string.Format("{0}\\{1}", path, filename);
            string[] _params = str.Split('#');
            Dictionary<string, string> paramArry = new Dictionary<string, string>();

            foreach (var s in _params)
            {
                string[] p = s.Split('=');
                if (p != null && p.Length == 2)
                {
                    paramArry.Add(p[0].Trim(), p[1].Trim());
                }
            }
            //             _params.First(s =>
            //             {
            //                 string[] p = s.Split('=');
            //                 paramArry.Add(p[0], p[1]);
            //                 return ;
            //             });

            StringBuilder stringBuilder = new StringBuilder();
            //stringBuilder.Append($"{27}{87}{1}");
           // stringBuilder.Append("         中国农业银行");//标题
            StreamReader sr = new StreamReader(file, Encoding.Default);
            XElement xele = XElement.Load(sr);
            // var xePrtRoot = xele.Elements("PrtRoot");//根节点
            string ss = formData(xele.Elements("TxtItem"), p =>
            {
                var v = paramArry.FirstOrDefault(x => x.Key.Trim().Equals(p.Trim()));//p前面有空格
                if (v.Key != null)
                {
                    paramArry.Remove(v.Key);
                    return v.Value;
                }
                return string.Empty;
            });//加载数据
            stringBuilder.Append(ss);

            //列表
            var xeListItemRoot = xele.Elements("ListItem");
            if (xeListItemRoot != null && xeListItemRoot.Count() != 0)
            {
                var conuAttr = xeListItemRoot.Attributes("CardNum");
                var xeItem = xeListItemRoot.Elements("TxtItem");
                var deCount = paramArry.FirstOrDefault(s => s.Key.Equals("CardNum"));//获取个数
                SysLog.d($"ListItem deCount={deCount}");
                string scount = deCount.Value;
                int listcount = int.Parse(scount);
                for (int i = 0; i < listcount; i++)
                {
                    string sss = formData(xeItem, p =>
                    {
                        string sp = paramArry.FirstOrDefault(x =>
                          {
                              string n = string.Format($"{p.Trim().Replace("$", "")}{i}");
                              string key = x.Key.Replace("$", "").Trim();
                              SysLog.d($"ListItem name={n} x.Key={key}");
                              return key.Equals(n);
                          }
                          ).Value;
                        return $"{sp}  ";
                    }
                    );//加载数据
                    stringBuilder.Append(sss);

                }
            }

            return stringBuilder.ToString();
            
        }


        private string formData(IEnumerable<XElement> xes, Func<string, string> func)
        {
            StringBuilder stringBuilder = new StringBuilder();
            foreach (var item in xes)//遍历TxtItem节点
            {
                var newRow = item.Attribute("NewRow");
                string _NewRow = "0";//是否另起一行打印
                string _Var = "0";//是否有值
                if (newRow != null)
                {
                    _NewRow = newRow.Value;//获取_newRow 属性的值
                }
                var vVar = item.Attribute("Var");
                if (vVar != null)
                {
                    _Var = vVar.Value;//获取Var 属性的值
                }

                var _txt = item.Value;//TxtItem节点的文本
                if (_NewRow != null && _NewRow.Equals("1"))
                {
                    stringBuilder.Append("\n");
                    stringBuilder.Append("");//向右挪点点
                }
                if (_Var != null && _Var.Equals("1"))//是否为值
                {
                    string v = func.Invoke(_txt); //_params.First(p => p == _txt);//用_params的值替换

                    stringBuilder.Append(v);
                }
                else
                {
                    stringBuilder.Append(_txt);
                }

            }
            return stringBuilder.ToString();
        }
    }
}
