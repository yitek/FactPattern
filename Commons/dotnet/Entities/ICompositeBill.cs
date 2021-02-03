using System;
using System.Collections.Generic;
using System.Text;

namespace FactPattern.Entities
{
    public interface ICompositeBill<TNode>: IBill, ICompositeEntity<TNode>
        where TNode:class,ICompositeEntity
    {
    }
}
