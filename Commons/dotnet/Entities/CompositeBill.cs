using System;
using System.Collections.Generic;
using System.Text;

namespace FactPattern.Entities
{
    public class CompositeBill<TNode>:Bill,ICompositeEntity<TNode>
        where TNode: class,ICompositeEntity
    {
        public CompositeBill() { }
        public CompositeBill(Guid id) : base(id) { }

        public string NodeCode { get; set; }

        public string NodePath { get; set; }

        public Guid? ParentId { get; set; }

        public TNode Parent { get; set; }

        public IList<TNode> Children { get; set; }
        IList<ICompositeEntity> ICompositeEntity.Children { get => throw new NotImplementedException(); set => throw new NotImplementedException(); }
        ICompositeEntity ICompositeEntity.Parent { get => this.Parent; set => this.Parent=(TNode)value; }
    }
}
