using FactPattern.Entities;
using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Text;

namespace FactPattern
{
    /// <summary>
    /// 用户对象类
    /// </summary>
    /// <typeparam name="T">主键类型</typeparam>
    public class User<T> :Entity<T>, IUser<T>
    {
        public User(T id, string name):base(id)
        {
            this.Name = name;
        }
        public User() { }

        /// <summary>
        /// 用户唯一名
        /// </summary>
        public string Name { get; set; }

        #region equals
        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        public virtual bool Equals([AllowNull] IUser<T> other)
        {
            if (other == null) return false;
            return this.Id.Equals<T>(other.Id);
        }
        public override bool Equals(object obj)
        {
           
            return base.Equals(obj) || (obj is IUser<T> other && other.Id.Equals<T>(this.Id));
           
        }

        public static bool operator ==(User<T> left, User<T> right) {
            return Entity<T>.PrimaryEqual(left,right);
        }

        public static bool operator !=(User<T> left, User<T> right) { 
            return !Entity<T>.PrimaryEqual(left, right);
        }
        #endregion

    }
    /// <summary>
    /// 以Guid为主键的用户对象类
    /// </summary>
    public class User:User<Guid>,IUser,IEquatable<IUser>
    {
        public User(Guid id, string name) : base(id,name){}
        public User() { }
        #region equals
        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        public override bool Equals([AllowNull] IUser<Guid> other)
        {
            return other != null && other.Id == this.Id;
        }

        public override bool Equals(object obj)
        {
            return base.Equals(obj) || (obj is IUser o && o.Id == this.Id);
        }

        public bool Equals([AllowNull] IUser other)
        {
            return other != null && other.Id == this.Id;
        }

        public static bool operator ==(User left, User right)
        {
            return left == right || (left?.Id==right?.Id);
        }

        public static bool operator !=(User left, User right)
        {
            return left != right && (left?.Id != right?.Id);
        }
        #endregion
    }

    /// <summary>
    /// 以字符为主键的用户对象类
    /// </summary>
    public class StringUser : User<string>, IStringUser {
        public StringUser(string id, string name) : base(id, name) { }
        public StringUser() { }
        #region equals
        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        public override bool Equals([AllowNull] IUser<string> other)
        {
            return other != null && other.Id == this.Id;
        }

        public override bool Equals(object obj)
        {
            return base.Equals(obj) || (obj is IStringUser o && o.Id == this.Id);
        }

        public bool Equals([AllowNull] IStringUser other)
        {
            return other != null && other.Id == this.Id;
        }

        public static bool operator ==(StringUser left, StringUser right)
        {
            return left == right || (left?.Id == right?.Id);
        }

        public static bool operator !=(StringUser left, StringUser right)
        {
            return left != right && (left?.Id != right?.Id);
        }
        #endregion
    }
    /// <summary>
    /// 以整数为主键的用户对象类
    /// </summary>
    public class IntUser : User<int>, IIntUser {
        public IntUser(int id, string name) : base(id, name) { }
        public IntUser() { }
        #region equals
        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        public override bool Equals([AllowNull] IUser<int> other)
        {
            return other != null && other.Id == this.Id;
        }

        public override bool Equals(object obj)
        {
            return base.Equals(obj)|| ( obj is IIntUser o && o.Id == this.Id);
        }

        public bool Equals([AllowNull] IIntUser other)
        {
            return other != null && other.Id == this.Id;
        }

        public static bool operator ==(IntUser left, IntUser right)
        {
            return left == right || (left?.Id == right?.Id);
        }

        public static bool operator !=(IntUser left, IntUser right)
        {
            return left != right && (left?.Id != right?.Id);
        }
        #endregion

        
    }

    /// <summary>
    /// 以整数为主键的用户对象类
    /// </summary>
    public class LongUser : User<long>, ILongUser
    {
        public LongUser(long id, string name) : base(id, name) { }
        public LongUser() { }

        #region equals
        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        public override bool Equals([AllowNull] IUser<long> other)
        {
            return other != null && other.Id == this.Id;
        }

        public override bool Equals(object obj)
        {
            return base.Equals(obj) || (obj is ILongUser o && o.Id == this.Id);
        }

        public bool Equals([AllowNull] ILongUser other)
        {
            return other != null && other.Id == this.Id;
        }

        public static bool operator ==(LongUser left, LongUser right)
        {
            return left == right || (left?.Id == right?.Id);
        }

        public static bool operator !=(LongUser left, LongUser right)
        {
            return left != right && (left?.Id != right?.Id);
        }
        #endregion
    }
}
