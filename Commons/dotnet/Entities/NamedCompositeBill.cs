using System;
using System.Collections.Generic;
using System.Text;

namespace FactPattern.Entities
{
    public class NamedCompositeBill<TNode>:CompositeBill<TNode>
        where TNode:class,ICompositeEntity
    {
        public NamedCompositeBill() { }
        public NamedCompositeBill(Guid id) : base(id) { }

        public string Name { get; set; }

        public string Description { get; set; }

    }

    public class NamedCompositeBill : NamedCompositeBill<NamedCompositeBill> {
        public NamedCompositeBill() { }
        public NamedCompositeBill(Guid id) : base(id) { }
    }
}
