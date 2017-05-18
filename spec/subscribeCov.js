/* global describe, it, before, after */

require('should')
const bacnet = require('../bacnet.js')
const tools = require('./tools')

const iface = tools.getSuitableBroadcastInterface()

describe('Subscribe COV', function () {
  var device
  after('Exit the device fork', function (done) {
    device.once('exit', done)
    device.exit()
  })
  before(function (done) {
    device = tools.deviceProcess({
      datalink: {
        iface: iface
      },
      device: true
    })
    device.once('up', done)
  })
  // my writes dont currently support arrays - only single values
  describe('subscribe its own device object', function () {
    it('confirmed', function () {
      subscribeOwnDeviceObjectPropertyTest('analog-input', 'confirmed')
      subscribeOwnDeviceObjectPropertyTest('multi-state-input', 'confirmed')
    })
    it('unconfirmed', function () {
      subscribeOwnDeviceObjectPropertyTest('analog-input', 'unconfirmed')
      subscribeOwnDeviceObjectPropertyTest('multi-state-input', 'unconfirmed')
    })
    function subscribeOwnDeviceObjectPropertyTest (objectType, confirmed) {
      it('can subscribe the \'' + objectType + '\' property', function (done) {
        device.once('subscribe-cov-ack', (property) => {
          done()
        })
        const objectId = bacnet.objectTypeToNumber(objectType)
        const instance = 2
        const pid = 0
        device.subscribeCov('127.0.0.1', objectId, instance, pid, confirmed)
        device.once('error', done)
      })
    }
  })
})
