using System;
using System.Collections.Generic;
using System.Text;

namespace FactPattern.Entities
{
    /// <summary>
    /// 命名实体对象类
    /// 有名称Name跟Description两个属性
    /// </summary>
    /// <typeparam name="TPrimary"></typeparam>
    public class NamedEntity<TPrimary> : Entity<TPrimary>, INamedEntity<TPrimary>
    {
        public NamedEntity() { }
        public NamedEntity(TPrimary id) : base(id) { }

        /// <summary>
        /// 名称
        /// </summary>
        public string Name { get; set; }
        /// <summary>
        /// 描述
        /// </summary>
        public string Description { get; set; }
    }
    /// <summary>
    /// 以Guid为主键的命名实体对象类
    /// 有名称Name跟Description两个属性
    /// </summary>
    /// <typeparam name="TPrimary"></typeparam>
    public class NamedEntity:NamedEntity<Guid>
    {
        public NamedEntity() { }
        public NamedEntity(Guid id) : base(id) { }
    }
}
