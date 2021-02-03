using System;

namespace FactPattern.Entities
{
    /// <summary>
    /// 命名实体对象接口
    /// 有名称Name跟Description两个属性
    /// </summary>
    /// <typeparam name="TPrimary"></typeparam>
    public interface INamedEntity<TPrimary>:IEntity<TPrimary>
    {
        /// <summary>
        /// 名称
        /// </summary>
        string Name { get; set; }
        /// <summary>
        /// 描述
        /// </summary>
        string Description { get; set; }
        
    }
    /// <summary>
    /// 以Guid为主键的命名实体对象接口
    /// 有名称Name跟Description两个属性
    /// </summary>
    public interface INamedEntity : INamedEntity<Guid> { }
}