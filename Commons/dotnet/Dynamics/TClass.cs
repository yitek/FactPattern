using System;
using System.Collections.Generic;
using System.Reflection;
using System.Text;

namespace FactPattern.TypeExtensions
{
    public class TClass:TObject1
    {
        

        public TClass(Type type) :base(type){
            this.properties = new Dictionary<string, TProperty>();
            var members = type.GetMembers();
            foreach (var member in members) {
                if (member.MemberType == System.Reflection.MemberTypes.Property)
                {
                    var tprop = new TProperty(this, member as PropertyInfo);
                    this.properties.Add(tprop.Name, tprop);
                }
                else if (member.MemberType == MemberTypes.Field) {
                    var tprop = new TProperty(this,member as FieldInfo);
                    this.properties.Add(tprop.Name,tprop);
                }
            }
        }

        

        Dictionary<string, TProperty> properties;

        //Dictionary<string, TMethod> methods;

        public TProperty this[string name] {
            get {
                this.properties.TryGetValue(name ,out TProperty prop);
                return prop;
            }
        }

    }
}
