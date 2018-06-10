using ABC.abstractFun;

namespace ABC.BackSplint
{
    public class BasicFunction : aFuns
    {
        public override void SetData(byte[] buffer)
        {
            base.SetData(buffer);
           //返回心跳确认包
            backData(null,0);
        }
    }
}
