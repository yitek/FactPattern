using System;
using System.Collections.Generic;
using System.Reflection;
using System.Linq.Expressions;
using System.Collections.Concurrent;
using System.Threading;

namespace FactPattern.TypeExtensions
{
    public class TObject1
    {
        readonly static object NULL = new object();
        

        public Type Type { get; private set; }

        public TObject1(Type type) {
            this.Type = type;
            Expression<Func<object>> createObjectLamda;
            Expression<Func<object, bool>> hasValueLamda;
            var hasValueParExpr = Expression.Parameter(typeof(object), "obj");
            #region class
            if (this.Type.IsByRef)
            {
                
                this.UnderlayType = this.NullableType = this.Type;
                this.DefaultUnderlayValue = null;
                createObjectLamda = Expression.Lambda<Func<object>>(Expression.Convert(Expression.New(type),typeof(object)));

                this.CreateNullable = this.CreateUnderlay = this.CreateNullable = createObjectLamda.Compile();
                this.CreateDefaultValue = () => null;
                this.HasValue = (obj) => obj != null;
                return;
            }
            #endregion

            #region isNullable and hasValue
            this.IsNullable = this.Type.GUID == GeneralNullableType.GUID;
            if (this.IsNullable)
            {
                this.UnderlayType = type.GetGenericArguments()[0];
                this.NullableType = type;
                hasValueLamda = Expression.Lambda<Func<object, bool>>(
                    Expression.Condition(
                        Expression.Equal(hasValueParExpr, null)
                        , Expression.Constant(false, typeof(bool))
                        , Expression.Property(Expression.Convert(hasValueParExpr, type), "HasValue")
                    )
                    , hasValueParExpr
                );
                this.HasValue= hasValueLamda.Compile();
            }
            else {
                this.UnderlayType = type;
                this.NullableType = GeneralNullableType.MakeGenericType(type);
                hasValueLamda = Expression.Lambda<Func<object, bool>>(
                    Expression.Condition(
                        Expression.Equal(hasValueParExpr, null)
                        , Expression.Constant(false, typeof(bool))
                        , Expression.TypeIs(hasValueParExpr, type)
                    )
                    , hasValueParExpr
                );
                this.HasValue = hasValueLamda.Compile();
            }
            #endregion

            #region underlay value&create
            this.DefaultUnderlayValue = GetRawDefaultValue(this.UnderlayType);
            if (this.DefaultUnderlayValue == null)
            {
                createObjectLamda = Expression.Lambda<Func<object>>(Expression.Convert(Expression.New(this.UnderlayType), typeof(object)));
                this.CreateUnderlay = createObjectLamda.Compile();
            }
            else {
                this.CreateUnderlay = () => this.DefaultUnderlayValue;
            }
            #endregion
            #region nullable value&& create

            createObjectLamda = Expression.Lambda<Func<object>>(Expression.Convert(Expression.New(this.NullableType), typeof(object)));
            this.CreateNullable = createObjectLamda.Compile();

            if (this.IsNullable) this.CreateDefaultValue = this.CreateObject = this.CreateNullable;
            else this.CreateDefaultValue = this.CreateObject = this.CreateUnderlay;
            #endregion


        }


        public object DefaultValue {
            get;private set;
        }

        public Type UnderlayType { get; private set; }


        public Type NullableType { get; private set; }

        
        public object DefaultUnderlayValue { get; private set; }

        public bool IsNullable{get;private set;}

        public Func<object> CreateNullable { get; private set; }

        public Func<object> CreateUnderlay { get; private set; }

        public Func<object> CreateObject { get; private set; }

        public Func<object> CreateDefaultValue { get; private set; }

        public Func<object, bool> HasValue { get; private set; }

        public object ConvertTo(Type type, object src) {
            throw new NotImplementedException();
        }

        #region 对外入口
        readonly static SortedDictionary<int, TObject1> caches;
        readonly static ReaderWriterLockSlim locker = new ReaderWriterLockSlim();
        public static TObject1 Resolve(Type type) {
            throw new NotImplementedException();
        }

        static void InitStatics() { 
            
        }
        #endregion


        public static object GetRawDefaultValue(Type type) {
            
            if (type == typeof(byte)) return (byte)0;
            if (type == typeof(short)) return (short)0;
            if (type == typeof(ushort)) return (ushort)0;
            if (type == typeof(int)) return 0;
            if (type == typeof(uint)) return 0u;
            if (type == typeof(long)) return 0L;
            if (type == typeof(ulong)) return (ulong)0;
            if (type == typeof(float)) return 0f;
            if (type == typeof(double)) return 0d;
            if (type == typeof(decimal)) return new decimal(0);
            if (type == typeof(char)) return '\0';
            if (type == typeof(bool)) return false;
            if (type == typeof(DateTime)) return new DateTime(0);
            if (type == typeof(Guid)) return Guid.Empty;
            if (type.IsEnum) return 0;
            return null;
        }

        
        public readonly static Type GeneralNullableType = typeof(Nullable<>);
        public readonly static Type TypeType = typeof(Type);


        public readonly static PropertyInfo HasValuePropertyInfo = GeneralNullableType.GetProperty("HasValue");
    }
}
