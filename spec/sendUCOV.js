require('should')
const bacnet = require('../bacnet.js')
const tools = require('./tools')

const iface = tools.getSuitableBroadcastInterface()

function getDevice() {
  return tools.deviceProcess({
    datalink: {
      iface: iface
    },
    device: true
  })
}

function sendUCovTest(objType, propKey, val) {
  device.sendUCov(0, 260001,
    bacnet.objectTypeToNumber(objType),
    bacnet.propertyKeyToNumber(propKey),
    new bacnet.BacnetValue(val))
}

describe('Send Unconfirmed Cov', function () {
  const device = function () {

    before(function(done) {
      device.once('up', done)
    })

    after(function(done) {
      device.once('exit', done)
      device.exit()
    })

    return getDevice()
  }()

  describe('sending its own device object', function () {
    sendUCovTest('device', 'system-status', 1)
  })
})
