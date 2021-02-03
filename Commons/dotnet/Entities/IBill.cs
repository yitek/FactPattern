using System;

namespace FactPattern.Entities
{
    /// <summary>
    /// 业务单据
    /// </summary>
    public interface IBill:IEntity
    {
        /// <summary>
        /// 过程编号
        /// 该Bill可能是某个大流程中的其中一个业务单据
        /// </summary>
        string ProcessId { get; set; }
        /// <summary>
        /// 业务编号
        /// 一般是显示用，将日期戳显示出来
        /// </summary>
        string BillCode { get; set; }
        /// <summary>
        /// 状态码
        /// </summary>
        int StatusCode { get; set; }
        /// <summary>
        /// 所有者Id
        /// </summary>
        Guid? OwnerId { get; set; }
        /// <summary>
        /// 所有者唯一名
        /// </summary>

        string OwnerName { get; set; }
        /// <summary>
        /// 所有者对象
        /// </summary>
        IUser Owner { get; set; }
        #region 创建
        /// <summary>
        /// 创建时间
        /// </summary>
        DateTime CreateTime { get; set; }
        /// <summary>
        /// 创建者Id
        /// </summary>
        Guid CreatorId { get; set; }
        /// <summary>
        /// 创建者唯一名
        /// </summary>
        string CreatorName { get; set; }
        /// <summary>
        /// 创建者对象
        /// </summary>
        IUser Creator { get; }
        #endregion

        #region 修改
        /// <summary>
        /// 最后修改时间
        /// </summary>
        DateTime? ModifyTime { get; set; }
        /// <summary>
        /// 修改者Id
        /// </summary>
        
        Guid? ModifierId { get; set; }
        /// <summary>
        /// 修改者唯一名
        /// </summary>
        string ModifierName { get; set; }
        /// <summary>
        /// 修改者对象
        /// </summary>
        IUser Modifier { get; set; }

        #endregion

        #region 关闭
        /// <summary>
        /// 关闭时间
        /// </summary>
        DateTime? CloseTime { get; set; }
        /// <summary>
        /// 关闭者Id
        /// </summary>
        Guid? CloserId { get; set; }
        /// <summary>
        /// 关闭者唯一名
        /// </summary>
        string CloserName { get; set; }

        /// <summary>
        /// 关闭者
        /// </summary>
        IUser Closer { get; set; }
        #endregion
    }
}