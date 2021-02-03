using System;
using System.Collections.Generic;
using System.Text;

namespace FactPattern.Entities
{
    public class NamedBill:Bill
    {
        public NamedBill() { }
        public NamedBill(Guid id, IUser creator) : base(id, creator) { }

        public string Name { get; set; }

        public string Description { get; set; }
    }
}
