using System;
using System.Collections.Generic;
using System.Linq.Expressions;
using System.Reflection;
using System.Text;

namespace FactPattern.TypeExtensions
{
    public class TProperty:TObject1
    {
        public TClass Class { get; private set; }
        public TProperty(TClass cls, FieldInfo info):base(info.FieldType) {
            this.Class = cls;
            this.FieldInfo = info;
            this.Name = info.Name;
            this.Init();
        }
        public TProperty(TClass cls, PropertyInfo info) : base(info.PropertyType)
        {
            this.Class = cls;
            this.PropertyInfo = info;
            this.Name = info.Name;
            this.Init();
        }
        void Init() {
            this.InitGetValue();
            this.InitSetValue();
        }

        void InitGetValue() {
            var target = Expression.Parameter(typeof(object),"target");
            var typedTarget = Expression.Parameter(this.Class.Type,"ttarget");
            var codes = new List<Expression>();

            // 赋值 var typedTarget = target as TTarget;
            codes.Add(Expression.Assign(typedTarget,Expression.Convert(target,this.Class.Type)));
            // 类型不对，返空 if(typedTarget==null) return null;
            codes.Add(Expression.Condition(
                Expression.Equal(typedTarget,Expression.Constant(null))
                ,Expression.Constant(null)
                ,Expression.Convert(Expression.PropertyOrField(typedTarget,this.Name),typeof(object))
            ));

            var getValueLamda = Expression.Lambda<Func<object, object>>(Expression.Block(codes), target);
            this.GetValue = getValueLamda.Compile();
        }

        void InitSetValue()
        {
            var target = Expression.Parameter(typeof(object), "target");
            var value = Expression.Parameter(typeof(object),"value");
            var typedTarget = Expression.Parameter(this.Class.Type, "ttarget");
            var isAutoConvert = Expression.Parameter(typeof(bool),"isAutoConvert");
            var retLabel = Expression.Label();
            var codes = new List<Expression>();

            // 赋值 var typedTarget = target as TTarget;
            codes.Add(Expression.Assign(typedTarget, Expression.Convert(target, this.Class.Type)));
            // 类型不对，返空 if(typedTarget==null) return null;
            codes.Add(Expression.IfThen(
                Expression.Equal(typedTarget, Expression.Constant(null))
                , Expression.Return(retLabel)
            ));
            // if(value==null)
            var ifValueIsNullCodes = new List<Expression>();
            ifValueIsNullCodes.Add(Expression.Assign(Expression.PropertyOrField(typedTarget, this.Name), Expression.Constant(this.CreateDefaultValue())));
            ifValueIsNullCodes.Add(Expression.Return(retLabel));

            // if(value!=null)
            codes.Add( Expression.IfThenElse(
                Expression.Equal(value,Expression.Constant(null)),
                Expression.Block(ifValueIsNullCodes),
                Expression.Assign(
                    Expression.PropertyOrField(typedTarget, this.Name)
                    ,Expression.Convert(value,this.Type)
                )
            ));
            codes.Add(Expression.Label(retLabel));

            var setValueLamda = Expression.Lambda<Action<object, object,bool>>(Expression.Block(codes), target,value,isAutoConvert);
            this.SetValue = setValueLamda.Compile();
        }



        public string Name { get; private set; }
        public FieldInfo FieldInfo { get; private set; }
        public PropertyInfo PropertyInfo { get; private set; }

        public Func<object, object> GetValue { get; private set; }

        

        public Action<object, object,bool> SetValue { get; private set; }
    }
}
