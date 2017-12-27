// start.js

Page({
  data: {
    opacity: 1,
    disabled: false,
    threshold: 0,
    rule: 'up',
  },
  send: function () {
    const requestTask = wx.request({
      url: 'https://api.heclouds.com/devices/23349080/datapoints?datastream_id=youRenOrNot',
      header: {
        'content-type': 'application/json',
        'api-key': 'hXmpGJwtjjNcM=ZzS2sWImPrc5s='
      },
      success: function (res) {
        var app = getApp()
        var that = this
      
        app.globalData.YouRenOrNot = res.data.data.datastreams[0]
        var a=app.globalData.YouRenOrNot.datapoints[0].value
        console.log(app.globalData.YouRenOrNot)
        if (a==1) {
            wx.showModal({
              title: '啊哦！',
              content: '这个位置已经被占啦！',
              success: function (res) {
                if (res.confirm) {
                  console.log('用户点击确定')
                } else if (res.cancel) {
                  console.log('用户点击取消')
                }
              }
            })
          }
        else if (a == 0) {
            wx.showModal({
              title: '恭喜～',
              content: '这个座位暂时还没主。',
              success: function (res) {
                if (res.confirm) {
                  console.log('用户点击确定')
                } else if (res.cancel) {
                  console.log('用户点击取消')
                }
              }
            })
          }
      },

      fail: function (res) {
        console.log("fail!!!")
      },

      complete: function (res) {
        console.log("end")
      }
    })
  },

 /* change: function (e) {
    //当有输入时激活发送按钮，无输入则禁用按钮（***之后可用于实现查看某个教室的座位被占情况***）
    if (e.detail.value != "") {
      this.setData({
        threshold: e.detail.value,
        opacity: 1,
        disabled: false,
      })
    } else {
      this.setData({
        threshold: 0,
        opacity: 0.4,
        disabled: true,
      })
    }
  },*/

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {

  },

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {

  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {

  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {

  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload: function () {

  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {

  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {

  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {

  }
})