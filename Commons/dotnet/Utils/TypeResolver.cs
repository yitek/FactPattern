using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Reflection;
using System.IO;
namespace FactPattern.Utils
{
    public static class TypeResolver
    {
        readonly static SortedDictionary<string, Type> namedTypes = new SortedDictionary<string, Type>();
        readonly static List<string> resolvedPaths = new List<string>();
        //readonly static ConcurrentDictionary<int, Type> loadedTypes = new ConcurrentDictionary<int, Type>();
        static bool isLoadedResolved;

        readonly static System.Threading.ReaderWriterLockSlim locker = new System.Threading.ReaderWriterLockSlim(System.Threading.LockRecursionPolicy.NoRecursion);
        public static Type Resolve(string typename, string dllPath = null)
        {
            typename = typename.ToLower();
            if (dllPath == null) dllPath = AppDomain.CurrentDomain.BaseDirectory;
            
            locker.EnterUpgradeableReadLock();

            try
            {
                if (namedTypes.TryGetValue(typename, out Type rtype)) return rtype;
                if (!isLoadedResolved)
                {
                    locker.EnterWriteLock();
                    try
                    {
                        if (!isLoadedResolved)
                        {
                            rtype = ResolveLoadedAssemblies(typename);
                            isLoadedResolved = true;
                            if (rtype != null) return rtype;
                        }
                    }
                    finally
                    {
                        locker.ExitWriteLock();
                    }
                }
                var pathKey = dllPath.ToLower();
                if (resolvedPaths.Contains(pathKey)) return null;
                locker.EnterWriteLock();
                try
                {
                    var filenames = Directory.GetFiles(dllPath, "*.dll");
                    foreach (var filename in filenames)
                    {
                        var dllFile = Path.Combine(dllPath, filename);
                        Assembly asm = null;
                        try
                        {
                            Assembly.LoadFrom(dllFile);
                        }
                        catch
                        {
                            continue;
                        }
                        resolvedPaths.Add(dllFile.ToLower());
                        var types = asm.GetTypes();
                        foreach (var type in types)
                        {
                            var name = type.FullName.ToLower();
                            if (name == typename) rtype = type;
                            namedTypes.Add(name, type);
                        }
                    }
                    return rtype;
                }
                finally
                {
                    locker.ExitWriteLock();
                }
            }
            finally
            {
                locker.ExitUpgradeableReadLock();
            }
        }

        static Type ResolveLoadedAssemblies(string typename)
        {
            var asms = AppDomain.CurrentDomain.GetAssemblies();
            Type requiredType = null;
            foreach (var asm in asms)
            {
                var types = asm.GetTypes();
                foreach (var type in types)
                {
                    var name = type.FullName.ToLower();
                    if (name == typename) requiredType = type;
                    namedTypes.Add(name, type);
                }
            }
            return requiredType;
        }
    }
}
