using System;
using System.Collections.Generic;
using System.Linq.Expressions;
using System.Reflection;
using System.Text;

namespace FactPattern.Dynamics.Convertors
{
    public class Convertor
    {
        public Type Type { get; private set; }

        public Convertor(Type type) {
            this.Type = type;
            //decimal
            //if (type == typeof(DateTime)) return new DateTime();
            //if (type == typeof(Guid)) return Guid.Empty;
            //if (type.IsEnum) return 0;
            this.ThisType = Expression.Constant(this.Type,TObject.TypeType);
            
        }

        protected Expression ThisType { get; set; }

        
        public static bool CanUseConvert(Type type) {
            if (type == typeof(byte) || type == typeof(short) || type == typeof(ushort) || type == typeof(int) || type == typeof(uint) || type == typeof(long) || type == typeof(ulong) || type == typeof(double) || type == typeof(float) || type == typeof(bool) || type == typeof(char)) return true;
            return false;
        }

        static readonly MethodInfo CanUseConvertMethodInfo = typeof(Convertor).GetMethod("CanUseConvert");

        

        public int FromObject(object value)
        {
            Type type;
            int? nValue;
            if (value == null) return 0;
            if ((type = value.GetType()) == this.Type || CanUseConvert(type)) return (int)value;
            if (type == typeof(decimal)) return decimal.ToInt32((decimal)value);
            if (type == typeof(DateTime)) return (int)((DateTime)value).Ticks;
            if (type.GUID == TObject.GeneralNullableType.GUID) {
                type = type.GetGenericArguments()[0];
                if ((type = value.GetType()) == this.Type || CanUseConvert(type)) {
                    nValue = (int?)value;
                    return ((int?)value).HasValue ? nValue.Value : 0;
                }
                if (type == typeof(decimal)) return decimal.ToInt32((decimal)value);
                if (type == typeof(DateTime)) return (int)((DateTime)value).Ticks;
            }
            return 0;
        }

        






        public Expression ToInt(ParameterExpression value) {
            return value;
        }

    }
}
