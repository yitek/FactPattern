using System;
using System.Collections.Generic;
using System.Text;

namespace FactPattern.Entities
{
    public class CompositeEntity : Entity, ICompositeEntity
    {
        public CompositeEntity(Guid id) : base(id) { }

        public CompositeEntity() { }

        public string NodeCode { get; set; }

        public string NodePath { get; set; }

        public Guid? ParentId { get; set; }

        public ICompositeEntity Parent { get; set; }

        public IList<ICompositeEntity> Children { get; set; }

        public static string Encode(int num,int? width=null) {
            var sb = new StringBuilder();
            while (num != 0) {
                var rest = num % 36;
                if (rest < 10) sb.Insert(0, rest.ToString());
                else {
                    rest =rest- 10 + 'A';
                    sb.Insert(0,(char)rest);
                }
                num /= 36;
            }
            if (width != null) {
                var w = width.Value;
                if (sb.Length > w) throw new Exception(num.ToString() +"超出了指定宽度" + w.ToString() + ",无法被编码");
                for (int i = 0, j = w - sb.Length; i < j; i++) sb.Insert(0,'0');
            }
            return sb.ToString();
        }

        public static int Decode(string str) {
            int num = 0;
            int bas = 1;
            for (int i = str.Length - 1; i >= 0; i--) {
                var ch = str[i];
                int val;
                if (ch >= '0' && ch <= '9')
                {
                    val = (ch - '0') * bas;
                }
                else if (ch >= 'A' && ch <= 'Z')
                {
                    val = (ch - 'A' + 10) * bas;
                }
                else throw new FormatException("不正确的编码字符串，不能出现0-9，A-Z以外的字符");
                num += val;
                bas *= 32;
            }
            return num;
        }
    }

    public class CompositeEntity<TNode> : CompositeEntity,ICompositeEntity<TNode>
        where TNode :class,ICompositeEntity
    {
        public CompositeEntity(Guid id) : base(id) { }

        public CompositeEntity() { }

        

        public new TNode Parent { get; set; }

        public new IList<TNode> Children { get; set; }

    }



}
