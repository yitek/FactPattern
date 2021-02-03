using System;
using System.Collections.Generic;
using System.Linq.Expressions;
using System.Text;

namespace FactPattern.Dynamics
{
    public class TObject<T>:TObject
    {
        public TObject() : base(typeof(T)) {
            var obj = Expression.Parameter(TObject.ObjectType,"obj");
            if (this.Type.IsByRef)
            {
                this.CreateObject = Expression.Lambda<Func<T>>(Expression.New(this.Type)).Compile();
                this.CreateDefaultValue = () => default(T);
                this.HasValue = Expression.Lambda<Func<T, bool>>(Expression.NotEqual(obj, Expression.Constant(null, this.Type))).Compile();

            }
            else if (this.IsNullable)
            {
                this.CreateObject = this.CreateDefaultValue = () => default(T);
                var hasValueParExpr = obj;
                this.HasValue = Expression.Lambda<Func<T, bool>>(Expression.Property(hasValueParExpr, "HasValue"), hasValueParExpr).Compile();
            }
            else {
                this.CreateDefaultValue = this.CreateObject = () => default(T);
                this.HasValue = (obj) => true;
            }
            
        }

        #region orignal


        public new Func<T> CreateObject { get; protected set; }

        public new Func<T> CreateDefaultValue { get; protected set; }

        #endregion

        #region nullable

        public new Func<T, bool> HasValue { get; private set; }

        #endregion
    }
}
