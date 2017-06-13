/* global describe, it */

require('should')
const bacnet = require('../bacnet.js')
const tools = require('./tools')

const iface = tools.getSuitableBroadcastInterface()

describe('Subscribe COV', function () {
  subscribe('analog-input', 'confirmed')
  // subscribe('analog-input', 'unconfirmed')

  function subscribe (objType, confirmed) {
    it(`Subscribe COV for ${objType}(${confirmed})`, function (done) {
      const serverDeviceId = 260001
      const server = bacnet.init({
        datalink: {
          iface: iface
        },
        device: true,
        device_instance_id: serverDeviceId
      })

      server.once('iam', () => {
        const client = bacnet.init({
          datalink: {
            iface: iface,
            port: 0xBAC1,
            bbmd_port: 0xBAC0,
            bbmd_address: '127.0.0.1'
          },
          device: false,
          device_instance_id: 260002
        }).once('subscribe-cov-ack', done)

        const objectId = bacnet.objectTypeToNumber(objType)
        const instance = 0
        const pid = 0
        client.once('iam', (iam) => {
          client.subscribeCov(serverDeviceId, objectId, instance, pid, confirmed).should.equal(1)
        }).whois()
      }).whois()
    })
  }
})
