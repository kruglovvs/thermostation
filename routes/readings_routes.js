const Router = require('express');
const router = new Router();
const readingsConroller = require('../controller/readings_controller');

router.post('/readings', readingsConroller.addReadings);
router.get('/readings', readingsConroller.getReadings);
router.get('/readings/:id', readingsConroller.getOneReadings);
router.put('/readings', readingsConroller.updateReadings);
router.delete('/readings/:id', readingsConroller.deleteReadings);

module.exports = router;