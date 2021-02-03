using System;
using System.Collections.Generic;
using System.Text;

namespace FactPattern.Entities
{
    public interface INamedCompositeBill<TNode>:INamedBill,ICompositeBill<TNode>
        where TNode:class,ICompositeEntity
    {
    }

    public interface INamedCompositeBill : INamedBill, ICompositeBill<INamedCompositeBill> { 
        
    }
}
