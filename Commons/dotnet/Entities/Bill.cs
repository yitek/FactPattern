using System;
using System.Collections.Generic;
using System.Text;

namespace FactPattern.Entities
{
    /// <summary>
    /// 带着Create/Modify/Close属性的实体类
    /// </summary>
    /// <typeparam name="Guid"></typeparam>
    public class Bill : Entity, IBill
    {
        public Bill() { }

        public Bill(Guid id) : base(id) { }

        public Bill(Guid id, IUser creator) : base(id)
        {
            this.CreatorId = creator.Id;
            this.CreatorName = creator.Name;
            this.CreateTime = DateTime.Now;
        }
        /// <summary>
        /// 大流程Id
        /// </summary>
        public string ProcessId { get; set; }
        /// <summary>
        /// 状态码
        /// </summary>
        public int StatusCode { get; set; }

        /// <summary>
        /// 业务编号，一般带着日期戳显示用
        /// </summary>
        public string BillCode { get; set; }

        #region create
        /// <summary>
        /// 创建时间
        /// </summary>
        public DateTime CreateTime { get; set; }
        Guid creatorId;
        /// <summary>
        /// 创建者Id
        /// </summary>
        public Guid CreatorId { get { return this.creatorId; } set { this.creatorId = value; this.creator = null; } }

        string creatorName;
        /// <summary>
        /// 创建者唯一名
        /// </summary>
        public string CreatorName { get { return this.creatorName; } set { this.creatorName = value; this.creator = null; } }

        IUser creator;
        /// <summary>
        /// 创建者
        /// </summary>
        public IUser Creator
        {
            get
            {
                if (this.creator == null)
                {
                    if (this.CreatorId.IsDefault<Guid>()) return null;
                    this.creator = new User(this.CreatorId, this.CreatorName);
                }
                return this.creator;
            }
        }
        #endregion

        #region own
        Guid? ownerId;
        /// <summary>
        /// 所有者Id
        /// </summary>
        public Guid? OwnerId { get { return this.ownerId; } set { this.ownerId = value; this.owner = null; } }

        string ownerName;
        /// <summary>
        /// 所有者唯一名
        /// </summary>
        public string OwnerName { get { return this.ownerName; } set { this.ownerName = value; this.owner = null; } }

        IUser owner;
        /// <summary>
        /// 所有者
        /// </summary>
        public IUser Owner
        {
            get
            {
                if (this.owner == default)
                {
                    if (this.OwnerId == null) return default;
                    this.owner = new User(this.OwnerId.Value, this.OwnerName);
                }
                return this.owner;
            }
            set
            {
                if (value == null)
                {
                    this.ownerId = default;
                    this.ownerName = default;
                    this.owner = default;
                }
                else
                {
                    this.ownerId = value.Id;
                    this.ownerName = value.Name;
                    this.owner = value;
                }
            }
        }
        #endregion
        #region 修改

        /// <summary>
        /// 修改时间
        /// </summary>
        public DateTime? ModifyTime { get; set; }

        Guid? modifierId;
        /// <summary>
        /// 修改者Id
        /// </summary>
        public Guid? ModifierId { get { return this.modifierId; } set { this.modifierId = value; this.modifier = null; } }

        string modifierName;
        /// <summary>
        /// 修改者唯一名
        /// </summary>
        public string ModifierName { get { return this.modifierName; } set { this.modifierName = value; this.modifier = null; } }


        IUser modifier;
        /// <summary>
        /// 修改者
        /// </summary>
        public IUser Modifier
        {
            get
            {
                if (this.modifier == default)
                {
                    if (this.ModifierId == default) return null;
                    this.modifier = new User(this.ModifierId.Value, this.ModifierName);
                }
                return this.modifier;
            }
            set
            {
                this.ModifyTime = DateTime.Now;
                if (value == default)
                {
                    this.modifierId = default;
                    this.modifierName = default;
                    this.modifier = default;
                }
                else
                {
                    this.modifierId = value.Id;
                    this.modifierName = value.Name;
                    this.modifier = value;
                }
            }
        }
        #endregion

        #region 关闭
        /// <summary>
        /// 关闭时间
        /// </summary>

        public DateTime? CloseTime { get; set; }
        Guid? closerId;
        /// <summary>
        /// 关闭者唯一Id
        /// </summary>
        public Guid? CloserId { get { return this.closerId; } set { this.closerId = value; this.closer = null; } }

        string closerName;
        /// <summary>
        /// 关闭者唯一名
        /// </summary>
        public string CloserName { get { return this.closerName; } set { this.closerName = value; this.closer = null; } }


        IUser closer;
        /// <summary>
        /// 关闭者
        /// </summary>
        public IUser Closer
        {
            get
            {
                if (this.closer == default)
                {
                    if (this.closerId == null) return null;
                    this.closer = new User(this.closerId.Value, this.closerName);
                }
                return this.closer;
            }
            set
            {
                this.CloseTime = DateTime.Now;
                if (value == null)
                {
                    this.closer = default;
                    this.CloserName = default;
                    this.closer = default;
                }
                else
                {
                    this.closerId = value.Id;
                    this.closerName = value.Name;
                    this.closer = value;
                }
            }
            #endregion
        }
    }
}
