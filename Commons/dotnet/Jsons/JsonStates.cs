using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Dynamic;
using System.Text;

namespace WFlow
{
    public class JsonStates:DynamicObject
    {
        readonly JToken value;
        public JsonStates(object value) {
            if (value == null) this.value = null;
            else this.value = JToken.FromObject(value);
        }

        public override bool TryConvert(ConvertBinder binder, out object result)
        {
            if (this.value == null) {
                result = null;
                return true;
            }
            result = this.value.ToObject(binder.Type);
            return true;
        }

        public override bool TryGetMember(GetMemberBinder binder, out object result)
        {
            if (this.value == null) {
                result = null;
                return true;
            }
            var value = this.value[binder.Name];
            if (value == null || value.Type == JTokenType.Undefined || value.Type == JTokenType.Undefined) {
                result = null;
                return true;
            }
            result = new JsonStates(value);
            return true;
        }

        public override bool TrySetMember(SetMemberBinder binder, object value)
        {
            if (this.value == null) return true;
            this.value[binder.Name] = JToken.FromObject(value);
            return true;
        }

        public override bool TryGetIndex(GetIndexBinder binder, object[] indexes, out object result)
        {
            if (this.value == null) {
                result = null;
                return true;
            }
            if (indexes.Length != 1) throw new InvalidProgramException("只能有一个索引");
            var index = (int)indexes[0];
            var value = this.value[index];
            if (value == null) {
                result = null;
                return true;
            }
            result = new JsonStates(value);
            return true;
        }

        public override bool TrySetIndex(SetIndexBinder binder, object[] indexes, object value)
        {
            if (this.value == null) return true;
            if (indexes.Length != 1) throw new InvalidProgramException("只能有一个索引");
            var index = (int)indexes[0];
            this.value[index] = JToken.FromObject(value);
            return true;
        }

        

        public override bool TryInvokeMember(InvokeMemberBinder binder, object[] args, out object result)
        {
            throw new InvalidProgramException("不可以在该对象上调用成员函数");
        }

        public override bool TryInvoke(InvokeBinder binder, object[] args, out object result)
        {
            throw new InvalidProgramException("该对象不能当作函数调用");
        }

        

        public object GetValue() {
            return this.value;
        }
        public T GetValue<T>() {
            if (this.value == null) return default;
            return this.value.ToObject<T>();
        }
    }
}

