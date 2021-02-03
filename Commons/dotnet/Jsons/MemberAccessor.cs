using Newtonsoft.Json.Linq;
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Text;

namespace FactPattern.Jsons
{
    public class MemberAccessor
    {
        readonly List<string> memberNames;
        public MemberAccessor(string text) {
            this.memberNames = new List<string>();
            var txts = text.Split('.');
            foreach (var txt in txts) {
                var pname = txt.Trim();
                if (pname == null) throw new FormatException("错误的Json路径表达式:" + text);
                this.memberNames.Add(pname);
            }
            if(this.memberNames.Count==0) throw new FormatException("错误的Json路径表达式:" + text);
        }
        public JToken Get(JToken input) {
            if (input == null) return input;
            foreach (var pname in this.memberNames) {
                input = input[pname];
                if (input == null) return null;
            }
            return input;
        }
        public T Get<T>(JToken input) {
            var jval = this.Get(input);
            return jval==null?default:jval.ToObject<T>();
            
        }

        public void Set(JToken target, JToken value) {
            if (target == null) return;
            var lastIndex = this.memberNames.Count - 1;
            for (int i = 0; i < lastIndex; i++) {
                var pname = this.memberNames[i];
                var nextTarget = target[pname];
                if (nextTarget == null || nextTarget.Type != JTokenType.Object) {
                    nextTarget = new JObject();
                    target[pname] = nextTarget;
                }
                target = nextTarget;
            }
            target[this.memberNames[lastIndex]] = value;
        }

        public void Set(JToken target, object value) {
            this.Set(target,JToken.FromObject(value));
        }

        readonly static ConcurrentDictionary<string, MemberAccessor> caches = new ConcurrentDictionary<string, MemberAccessor>();

        public static MemberAccessor Resolve(string text) {
            return caches.GetOrAdd(text,(txt)=>new MemberAccessor(txt));
        }

        public static JToken GetValue(JToken target, string text) {
            return caches.GetOrAdd(text, (txt) => new MemberAccessor(txt)).Get(target);
        }

        public static T GetValue<T>(JToken target, string text)
        {
            var jval= caches.GetOrAdd(text, (txt) => new MemberAccessor(txt)).Get(target);
            return jval == null ? default : jval.ToObject<T>();
        }

        public static void SetValue(JToken target, string text,JToken value)
        {
            caches.GetOrAdd(text, (txt) => new MemberAccessor(txt)).Set(target,value);
        }

        public static void SetValue(JToken target, string text, object value)
        {
            caches.GetOrAdd(text, (txt) => new MemberAccessor(txt)).Set(target, JToken.FromObject(value));
        }

    }
}
