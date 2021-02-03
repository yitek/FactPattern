using System;
using System.Collections.Generic;
using System.Text;

namespace FactPattern
{
    public static class ObjectExtesion
    {
        public static bool IsDefault<T>(this T self) {
            var t = typeof(T);
            if (t == typeof(string)) return self?.ToString()==null;
            if (t.IsByRef) return (object)self ==null;
            return self.Equals(default);
        }

        public static bool Equals<T>(this T self, T other)
        {
            var t = typeof(T);
            if (t == typeof(string)) return self?.ToString() == other?.ToString();
            if (t.IsByRef) return (object)self == (object)other;
            return self.Equals(other);
        }
    }
}
