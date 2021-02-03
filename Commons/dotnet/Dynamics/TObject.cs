using System;
using System.Collections.Generic;
using System.Linq.Expressions;
using System.Reflection;
using System.Text;

namespace FactPattern.Dynamics
{
    public class TObject
    {
       

        protected TObject(Type type)
        {
            if (type.IsByRef)
            {
                this.InitClass(type);
            }
            else {
                if (type.GUID == GeneralNullableType.GUID)
                {
                    this.InitNullable(type);
                }
                else {
                    
                    this.InitStruct(type);
                }
            }
           
        }
        void InitClass(Type type) {
            this.Type = 
            this.NullableType = 
            this.UnderlayType = type;
            var createObjectLamda = Expression.Lambda<Func<object>>(Expression.Convert(Expression.New(type), typeof(object)));

            this.CreateUnderlay = this.CreateNullable = createObjectLamda.Compile();
            this.CreateDefaultValue = () => null;

            #region hasValue
            var hasValueobjExpr = Expression.Parameter(typeof(object), "obj");
            var hasValueLamda = Expression.Lambda<Func<object, bool>>(
                    Expression.Condition(
                        // obj == null?
                        Expression.Equal(hasValueobjExpr,Expression.Constant(null))
                        // return false
                        , Expression.Constant(false, typeof(bool))
                        // return $type.isAssingableFrom()
                        , Expression.Call(
                            Expression.Constant(type, TypeType)
                            , IsAssignableFromMethodInfo
                            // obj.getType()
                            , Expression.Call(hasValueobjExpr, GetTypeMethodInfo)
                        )
                    )
                    , hasValueobjExpr
                );
            this.HasValue = hasValueLamda.Compile();
            #endregion
        }
        void InitNullable(Type type) {
            this.IsNullable = true;
            Expression<Func<object>> createLamda;
            #region orignal
            this.Type = type;
            createLamda = Expression.Lambda<Func<object>>(Expression.Convert(Expression.New(type), typeof(object)));
            this.CreateObject = createLamda.Compile();
            this.CreateDefaultValue = this.CreateObject;
            #endregion

            #region nullable
            this.NullableType = type;
            this.CreateNullable = this.CreateObject;
            var hasValueParExpr = Expression.Parameter(typeof(object), "obj");
            var hasValueLamda = Expression.Lambda<Func<object, bool>>(
                Expression.Condition(
                    Expression.Equal(hasValueParExpr, null)
                    , Expression.Constant(false, typeof(bool))
                    , Expression.Condition(
                        Expression.Equal(Expression.Call(hasValueParExpr, GetTypeMethodInfo), Expression.Constant(this.NullableType, TypeType))
                        , Expression.Constant(false, typeof(bool))
                        , Expression.Property(Expression.Convert(hasValueParExpr, type), "HasValue")
                        )

                )
                , hasValueParExpr
            );
            this.HasValue = hasValueLamda.Compile();
            #endregion

            #region create underlay
            this.UnderlayType = type.GetGenericArguments()[0];
            object underlayValue = GetPrimaryTypeDefaultValue(this.UnderlayType);
            if (underlayValue != null)
            {
                this.CreateUnderlay = () => underlayValue;
            }
            else {
                createLamda = Expression.Lambda<Func<object>>(Expression.Convert(Expression.New(this.UnderlayType), typeof(object)));
                this.CreateUnderlay = createLamda.Compile();
            }
            #endregion
        }

        void InitStruct(Type type)
        {

            Expression<Func<object>> createLamda;
           
            #region orignal
            this.Type = type;
            object defaultValue = GetPrimaryTypeDefaultValue(type);
            if (defaultValue != null)
            {
                this.CreateDefaultValue = this.CreateObject = this.CreateUnderlay = () => defaultValue;
            }
            else {
                createLamda = Expression.Lambda<Func<object>>(Expression.Convert(Expression.New(type), typeof(object)));
                this.CreateDefaultValue = this.CreateObject = this.CreateUnderlay = createLamda.Compile();
            }
            
            #endregion

            #region nullable
            this.NullableType = GeneralNullableType.MakeGenericType(type);
            createLamda = Expression.Lambda<Func<object>>(Expression.Convert(Expression.New(this.NullableType), typeof(object)));
            this.CreateNullable = createLamda.Compile();
            #endregion
            #region hasValue
            var hasValueParExpr = Expression.Parameter(typeof(object), "obj");
            var hasValueLamda = Expression.Lambda<Func<object, bool>>(
                Expression.Condition(
                    Expression.Equal(hasValueParExpr, Expression.Constant(null,ObjectType))
                    , Expression.Constant(false, typeof(bool))
                    , Expression.Call(
                        Expression.Constant(type, TypeType)
                        , IsAssignableFromMethodInfo
                        , Expression.Call(hasValueParExpr, GetTypeMethodInfo)
                    )
                )
                , hasValueParExpr
            );
            this.HasValue = hasValueLamda.Compile();
            #endregion
        }


        

        #region orignal
        public Type Type { get; private set; }

        public Func<object> CreateObject { get; protected set; }

        public Func<object> CreateDefaultValue { get; protected set; }

        #endregion

        #region nullable
        public bool IsNullable { get; private set; }

        public Type NullableType { get; private set; }

        public Func<object> CreateNullable { get; private set; }

        public Func<object, bool> HasValue { get; private set; }

        #endregion

        #region underlay

        public Type UnderlayType { get; private set; }

        public object DefaultUnderlayValue { get; private set; }

        

        

        public Func<object> CreateUnderlay { get; private set; }


        #endregion

        #region convert
        
        #endregion

        #region misc

        public static object GetPrimaryTypeDefaultValue(Type type)
        {
            if (type.IsByRef) return null;
            if (type == typeof(int)) return 0;
            if (type == typeof(DateTime)) return new DateTime();
            if (type == typeof(Guid)) return Guid.Empty;
            if (type.IsEnum) return 0;
            if (type == typeof(decimal)) return new decimal();

            if (type == typeof(byte)) return (byte)0;
            if (type == typeof(short)) return (short)0;
            if (type == typeof(ushort)) return (ushort)0;
            //if (type == typeof(int)) return 0;
            if (type == typeof(uint)) return 0u;
            if (type == typeof(long)) return 0L;
            if (type == typeof(ulong)) return (ulong)0;
            if (type == typeof(float)) return 0f;
            if (type == typeof(double)) return 0d;
            if (type == typeof(char)) return '\0';
            //if (type == typeof(bool)) return false;
            //if (type == typeof(decimal)) return new decimal();
            //if (type == typeof(DateTime)) return new DateTime();
            //if (type == typeof(Guid)) return Guid.Empty;
            //if (type.IsEnum) return 0;

            return null;
        }

        public static object ExtractNullable(object value) {
            if (value == null) return null;
            var type = value.GetType();
            if (type == typeof(byte?)) return ((byte?)value).GetValueOrDefault();
            if (type == typeof(short?)) return ((short?)value).GetValueOrDefault();
            if (type == typeof(ushort?)) return ((ushort?)value).GetValueOrDefault();
            if (type == typeof(int?)) return ((int?)value).GetValueOrDefault();
            if (type == typeof(uint?)) return ((uint?)value).GetValueOrDefault();
            if (type == typeof(long?)) return ((long?)value).GetValueOrDefault();
            if (type == typeof(ulong?)) return ((ulong?)value).GetValueOrDefault();
            if (type == typeof(float?)) return ((float?)value).GetValueOrDefault();
            if (type == typeof(double?)) return ((double?)value).GetValueOrDefault();
            if (type == typeof(char?)) return ((char?)value).GetValueOrDefault();
            if (type == typeof(bool?)) return ((bool?)value).GetValueOrDefault();
            if (type == typeof(decimal?)) return ((byte?)value).GetValueOrDefault();
            if (type == typeof(DateTime?)) return ((DateTime?)value).GetValueOrDefault();
            if (type == typeof(Guid?)) return ((Guid?)value).GetValueOrDefault();
            if (type.IsEnum) return 0;
            return null;
            
        }

        public readonly static object Nothing = new object();

        public readonly static Type GeneralNullableType = typeof(Nullable<>);
        public readonly static Type TypeType = typeof(Type);
        public readonly static Type ObjectType = typeof(object);

        public readonly static MethodInfo GetTypeMethodInfo = ObjectType.GetMethod("GetType",Type.EmptyTypes);
        public readonly static MethodInfo IsAssignableFromMethodInfo = TypeType.GetMethod("IsAssignableFrom",new Type[] { TypeType });

        #endregion

        #region Resolve
        readonly static Dictionary<int, TObject> clzs;
        #endregion
    }
}
