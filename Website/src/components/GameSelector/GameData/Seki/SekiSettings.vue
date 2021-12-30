<template>
  <div>
    <b-form>
      <b-form-group id="pass_options">
        <b-form-checkbox-group
          v-model="pass_options"
          id="checkboxes-4"
        >
          <b-form-checkbox value="R">R пасует?</b-form-checkbox>
          <b-form-checkbox value="C">C пасует?</b-form-checkbox>
        </b-form-checkbox-group>
      </b-form-group>
    </b-form>
  </div>
</template>

<script>
export default {
  data() {
    return {
      pass_options: [],
    }
  },
  methods: {
    getSettings() {
      return {
        "pass_options": this.pass_options
        };
    },
  }
}
</script>
