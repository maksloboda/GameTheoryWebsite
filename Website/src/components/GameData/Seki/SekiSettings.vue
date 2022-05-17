<template>
  <div>
    <b-form>
      <b-form-group 
        id="pass_options"
        label="Пропуск хода:"
      >
        <b-form-checkbox-group
          v-model="pass_options"
          id="checkboxes-4"
        >
          <b-form-checkbox value="R">R пасует?</b-form-checkbox>
          <b-form-checkbox value="C">C пасует?</b-form-checkbox>
        </b-form-checkbox-group>
      </b-form-group>
      <b-row>
        <b-col sm="6">
          <b-form-group 
            id="table_width"
            label="Ширина таблицы:"
          >            
              <b-form-input
                v-model="table_width"
                id="table-width"
                type="number"
                min="0"
                max="10"
              >
              </b-form-input>
          </b-form-group>
        </b-col>
        <b-col sm="6">
          <b-form-group 
            id="table_height"
            label="Высота таблицы:"
          >
            <b-form-input
              v-model="table_height"
              id="table_height"
              type='number'
            >
            </b-form-input>
          </b-form-group>
        </b-col>
      </b-row>
    </b-form>
  </div>
</template>

<script>
export default {
  data() {
    return {
      pass_options: [],
      table_width: '4',
      table_height: '4'
    }
  },
  methods: {
    getSettings() {
      return {
        "pass_options": this.pass_options,
        "table_width": this.table_width,
        "table_height": this.table_height
        };
    },
  }
}
</script>
