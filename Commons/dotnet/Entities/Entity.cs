using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Text;

namespace FactPattern.Entities
{
    /// <summary>
    /// 实体对象基类
    /// </summary>
    /// <typeparam name="TPrimary">主键类型</typeparam>
    public class Entity<TPrimary> : IEntity<TPrimary>
    {
        public Entity() { }

        public Entity(TPrimary id)
        {
            this.Id = id;
        }
        /// <summary>
        /// 唯一主键
        /// </summary>
        public TPrimary Id { get; protected set; }

        

        /// <summary>
        /// 重置主键
        /// </summary>
        /// <param name="id"></param>
        public void SetAssignedId(TPrimary id)
        {
            this.Id = id;
        }

        public static bool PrimaryEqual(IEntity<TPrimary> left, IEntity<TPrimary> right)
        {
            if (left == right) return true;
            if (left == null || right == null) return false;
            return left.Id.Equals<TPrimary>(right.Id);
        }
        

    }
    /// <summary>
    /// 以Guid为主键的实体对象基类
    /// </summary>
    public class Entity:Entity<Guid>
    {
        public Entity() { }
        public Entity(Guid id) : base(id) { }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        public static bool operator ==(Entity left, Entity right) {
            return left == right || left?.Id == right?.Id;
        }
        public static bool operator !=(Entity left, Entity right) {
            return left != right && left?.Id != right?.Id;
        }

        public override bool Equals(object obj)
        {
            return base.Equals(obj) || ((obj as IEntity)?.Id == this.Id);
        }
    }
}
