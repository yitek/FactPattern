using System;
using System.Collections.Generic;

namespace FactPattern.Entities
{
    public interface ICompositeEntity:IEntity
    {
        IList<ICompositeEntity> Children { get; set; }
        string NodeCode { get; set; }
        string NodePath { get; set; }
        ICompositeEntity Parent { get; set; }
        Guid? ParentId { get; set; }
    }

    public interface ICompositeEntity<TNode> : ICompositeEntity
        where TNode : class,ICompositeEntity
    {
        new IList<TNode> Children { get; set; }
        
        new TNode Parent { get; set; }
        
    }
}