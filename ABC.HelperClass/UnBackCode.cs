using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ABC.HelperClass
{
    public class UnBackCode
    {
        public static int UnBackSexCode(string sex_name)
        {
            switch (sex_name)
            {
                case "未知的性别":
                    return 0;
                case "男":
                    return 1;
                case "女":
                    return 2;
                case "未说明的性别":
                    return 3;
                default:
                    return 0;

            }
        }

        public static int UnBackNationCode(string Nation_Name)
        {
            switch (Nation_Name)
            {
                case "汉":
                    return 1;
                case "蒙古":
                    return 2;
                case "回":
                    return 3;
                case "藏":
                    return 4;
                case "维吾尔":
                    return 5;
                case "苗":
                    return 6;
                case "彝":
                    return 7;
                case "壮":
                    return 8;
                case "布依":
                    return 9;
                case "朝鲜":
                    return 10;
                case "满":
                    return 11;
                case "侗":
                    return 12;
                case "瑶":
                    return 13;
                case "白":
                    return 14;
                case "土家":
                    return 15;
                case "哈尼":
                    return 16;
                case "哈萨克":
                    return 17;
                case "傣":
                    return 18;
                case "黎":
                    return 19;
                case "傈僳":
                    return 20;
                case "佤":
                    return 21;
                case "畲":
                    return 22;
                case "高山":
                    return 23;
                case "拉祜":
                    return 24;
                case "水":
                    return 25;
                case "东乡":
                    return 26;
                case "纳西":
                    return 27;
                case "景颇":
                    return 28;
                case "柯尔克":
                    return 29;
                case "土":
                    return 30;
                case "达斡尔":
                    return 31;
                case "仫佬":
                    return 32;
                case "羌":
                    return 33;
                case "布朗":
                    return 34;
                case "撒拉":
                    return 35;
                case "毛难":
                    return 36;
                case "仡佬":
                    return 37;
                case "锡伯":
                    return 38;
                case "阿昌":
                    return 39;
                case "普米":
                    return 40;
                case "塔吉克":
                    return 41;
                case "怒":
                    return 42;
                case "乌孜别克":
                    return 43;
                case "俄罗斯":
                    return 44;
                case "鄂温克":
                    return 45;
                case "德昂":
                    return 46;
                case "保安":
                    return 47;
                case "裕固":
                    return 48;
                case "京":
                    return 49;
                case "塔塔尔":
                    return 50;
                case "独龙":
                    return 51;
                case "鄂伦春":
                    return 52;
                case "赫哲":
                    return 53;
                case "门巴":
                    return 54;
                case "珞巴":
                    return 55;
                case "基诺":
                    return 56;
                case "穿青":
                    return 57;
                default:
                    return 97;
            }
        }
    }
}
