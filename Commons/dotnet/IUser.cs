using FactPattern.Entities;
using System;
using System.Collections.Generic;
using System.Text;

namespace FactPattern
{
    /// <summary>
    /// 用户对象接口
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public interface IUser<T>:IEntity<T>,IEquatable<IUser<T>>
    {
        /// <summary>
        /// 用户唯一名
        /// </summary>
        public string Name { get; }
    }
    /// <summary>
    /// 以Guid为主键的用户对象接口
    /// </summary>
    public interface IUser:IUser<Guid>,IEntity
    {
    }
    /// <summary>
    /// 以字符串为主键的用户对象接口
    /// </summary>
    public interface IStringUser : IUser<string>
    {
    }
    /// <summary>
    /// 以整数为主键的用户对象接口
    /// </summary>
    public interface IIntUser : IUser<int> { }

    public interface ILongUser : IUser<long> { }
}
