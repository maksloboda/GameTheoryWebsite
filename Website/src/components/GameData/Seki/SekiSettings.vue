<template>
  <div>
    <b-form>
      <b-form-group 
        id="pass_options"
        label="Pass options:"
      >
        <b-form-checkbox-group
          v-model="pass_options"
          id="checkboxes-4"
        >
          <b-row>
              <b-col sm="6">
                <b-form-checkbox value="R">R can pass</b-form-checkbox>
              </b-col>
              <b-col sm="6">
                <b-form-checkbox value="C">C can pass</b-form-checkbox>
              </b-col>
          </b-row>
        </b-form-checkbox-group>
      </b-form-group>
      <b-row>
        <b-col sm="6">
          <b-form-group 
            id="field_width"
            label="Field width:"
          >            
              <b-form-input
                v-model="field_width"
                id="field-width"
                type="number"
                min="1"
                max="10"
              >
              </b-form-input>
          </b-form-group>
        </b-col>
        <b-col sm="6">
          <b-form-group 
            id="field_height"
            label="Field height:"
          >
            <b-form-input
              v-model="field_height"
              id="field_height"
              type='number'
              min="1"
              max="10"
            >
            </b-form-input>
          </b-form-group>
        </b-col>
      </b-row>
      <b-card>
        <b-row v-for="i in Array(minMax(field_height, 1, 10)).keys()">
          <b-col v-for="j in Array(minMax(field_width, 1, 10)).keys()">
            field[{{i * field_width + j }}] = {{ field[ i * field_width + j ] }} :
            <b-form-input
              v-model="field[i * field_width + j]"
              type='number'
              min="0"
              max="100"
              class="field_cell"
            > </b-form-input>
          </b-col>
        </b-row>
      </b-card>
    </b-form>
  </div>
</template>

<style>
select.form-control {
  -moz-appearance: none;
   -webkit-appearance: none;
   appearance: none;
}
</style>

<script>
export default {
  data() {
    return {
      pass_options: [],
      field_width: 4,
      field_height: 4,
      field: Array(100).fill(1) 
    }
  },
  methods: {
    getSettings() {
      let w = this.minMax(this.field_width, 1, 10)
      let h = this.minMax(this.field_height, 1, 10)
      let f = Array(w * h)

      let c = 0;
      for (let i = 0; i < h; ++i) {
        for (let j = 0; j < w; ++j) {
          f[c++] = parseInt(this.field[i * w + j])
        }
      }
      return {
          pass_options: this.pass_options,
          field_width: w,
          field_height: h,
          field: f
        };
    },
    minMax(val, min, max) {
      if (parseInt(val) < min) { return min; }
      if (parseInt(val) > max) { return max; }
      return parseInt(val)
    }
  }
}
</script>
