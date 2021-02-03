using System;

namespace FactPattern.Entities
{
    /// <summary>
    /// 实体对象基类接口
    /// </summary>
    /// <typeparam name="TPrimary">主键类型</typeparam>
    public interface IEntity<TPrimary>
    {
        /// <summary>
        /// 唯一主键
        /// </summary>
        TPrimary Id { get; }
        /// <summary>
        /// 重置唯一主键
        /// </summary>
        /// <param name="id"></param>
        void SetAssignedId(TPrimary id);
    }
    /// <summary>
    /// 以Guid为主键的实体对象基类接口
    /// </summary>

    public interface IEntity : IEntity<Guid> { }
}